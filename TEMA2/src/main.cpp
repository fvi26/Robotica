#include <Arduino.h>
#include <string.h>

#define BAUD 28800
#define LED_ROSU 11
#define LED_VERDE 10
#define LED_ALBASTRU 9
#define BUTON_START 2
#define BUTON_difficulty 3
#define BACKSPACE 8

unsigned long TypingTime[3] = {5000, 4000, 3000}, GameTime = 33000, WordSelectionMoment = 0, DebounceTime = 300, StartDelay = 3000, StartError = 50;
char Word[30], TargetWord[30];
char WordList [10][30] = {"fereastra","ecou","intamplare","noapte","aventura","furtuna","calatorie","munte","refugiu","balsam"};
int index = 0, difficulty = 0, LedState = 0, BlinkCount = 0, score = 0;
char DifficultyNames[3][100] = {"\nEasy mode on\n", "\nMedium  mode on\n", "\nHard mode on\n"};
volatile unsigned long DifficultyButtonPressTime = 0, StartButtonPressTime = 0, GameStartMoment = 0, lastBlinkMoment = 0;
bool repaus = true, IsRunning = false;

//Functia seteaza culoarea led-ului RGB
void setRGB(int ValueRed, int ValueGreen, int valAlbastru) 
{
  analogWrite(LED_ROSU, ValueRed);
  analogWrite(LED_VERDE, ValueGreen);
  analogWrite(LED_ALBASTRU, valAlbastru);
}

//Functia primeste ca parametru un sir de caractere si verifica daca se potriveste cu Wordul tinta (cel care trebuie scris)
//Functia va returna 1 daca cele 2 cuvinte sunt identice, 0 daca literele corespund, dar sirul de caractere este incomplet,
//si -1 daca literele difera la un anumit punct
int VerifyWord(const char *WordToVerify)
{
  int lenght = strlen(WordToVerify);
  for(int i=0; i<lenght; i++)
  {
    if(WordToVerify[i] != TargetWord[i])
      return -1;
  }
  if(lenght == int(strlen(TargetWord)))
    return 1;
  return 0;
}

//Functia afiseaza scoreul si reseteaza LED-ul RGB
void finish()
{
  Serial.println("\nTerminat!\nscore:");
  Serial.println(score);
  Serial.println("\n");
  setRGB(100, 100, 100);
}

//Functia verifica daca timpul de joc (30 s) a expirat; daca da, atunci se apeleaza functia de finish si se trece in starea de repaus
void VerifyState()
{
  if(IsRunning && millis() - GameStartMoment > GameTime)
  {
    repaus = true;
    IsRunning = false;
    finish();
  }
}

//Functia este apelata in primele 3 secunde de la apasarea butonului de start; se ocupa de numaratoarea inversa pana la starta
//jocului si de clipitul LED-ului in cele 3 secunde
void start()
{
  //Verifica daca a trecut destul timp de la ultima modificare a starii LED-ului pentru a o putea modifica din nou
  if(millis() - lastBlinkMoment > StartDelay/6)
  {
    lastBlinkMoment = millis();
    setRGB(100 * LedState, 100 * LedState, 100 * LedState);
    LedState = (LedState + 1) % 2;

    //Daca LED-ul si-a schimbat starea de un numar par de ori mai mic decat 6 (inclusiv 0), se va afisa un numar, reprezentand
    //numaratoarea inversa pana la starta jocului
    if(!(BlinkCount % 2))
    {
      Serial.println(3 - BlinkCount/2 );
    }

    BlinkCount++;
  }

  //Cand LED-ul si-a schimbat starea de 6 ori (a clipit de 3 ori) se va selecta primul Word de tastat, LED-ul se va face verde,
  //si se reseteaza sirul de caracter "Word" in care se stocheaza ce se tasteaza
  if(BlinkCount == 6)
  {
    index = 0;
    strcpy(Word, "");
    strcpy(TargetWord,WordList[(random(10) + GameStartMoment) % 10]);
    Serial.println(TargetWord);
    WordSelectionMoment = millis();
    setRGB(0, 100, 0);
    BlinkCount++;
  }
}

//Functia initializeaza valori necesare pentru starta si IsRunninga jocului
void StartGame()
{
  GameStartMoment = millis();
  score = 0;

  //Necesara pentru animatia de inceput
  lastBlinkMoment = millis();
  BlinkCount = 0;
}

//Functie pentru intrerupere pe butonul de schimbat difficultya
void VerifyDifficultyButton()
{
  if(repaus)
  {
    if(millis() - DifficultyButtonPressTime > DebounceTime && digitalRead(BUTON_difficulty) == LOW)
    {
      DifficultyButtonPressTime = millis();
      difficulty = (difficulty + 1) % 3;
      Serial.println(DifficultyNames[difficulty]);
    }
  }
}

//Functie pentru intrerupere pe butonul de start
//Daca starea era de repaus, jocul va incepe; daca starea era de IsRunning, jocul se va finaliza
void VerifyStartButton()
{
  if(millis() - StartButtonPressTime > DebounceTime && digitalRead(BUTON_START) == LOW)
  {
    StartButtonPressTime = millis();
    repaus = !repaus;
    IsRunning = !IsRunning;
    if(IsRunning)
    {
      StartGame();
    }else
    {
      finish();
    }
  }
}

//Functie pentru citirea unui caracter de pe interfata seriala
//Caracterul citit va fi salavat in sirul de caractere "Word"
//Daca se apasa "Backspace" se va sterge ultimul caracter din "Word"
void citireLitera()
{
  char litera = Serial.read();
  if(int(litera) == BACKSPACE)
  {
    if(index > 0)
    {
      Word[index - 1] = NULL;
      index--;
    }
  }else
  {
    Word[index] = litera;
    index++;
    Word[index] = NULL;
  }
}

//Functia alege un Word nou ca tinta din WordList si memoreaza momentul cand acesta a fost ales
//Parametrul verdict determina daca Wordul anterior a fost scris corect in limita de timp
//Daca da atunci creste scoreul; altfel trimite un mesaj care anunta ca timpul a expirat
void alegereWordNou(int verdict)
{
  if(verdict)
  {
    score++;
    Serial.println("\n");
  }else
  {
    Serial.println("\nTimp expirat!\n");
  }
  strcpy(Word," ");
  index = 0;
  strcpy(TargetWord,WordList[(random(10) + GameStartMoment) % 10]);
  Serial.println(TargetWord);
  WordSelectionMoment = millis();
}

void setup()
{
    Serial.begin(BAUD);

    pinMode(LED_ROSU, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_ALBASTRU, OUTPUT);

    setRGB(100, 100, 100);

    pinMode(BUTON_START, INPUT_PULLUP);
    pinMode(BUTON_difficulty, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTON_START), VerifyStartButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTON_difficulty), VerifyDifficultyButton, FALLING);

    Serial.println("\nSetup complet\n");
}

void loop()
{
  VerifyState();

  if(IsRunning)
  {
    if(millis() - GameStartMoment <= StartDelay + StartError)
    {
      start();
    }else
    {
      if(Serial.available()) 
      {
          citireLitera();
          
          if(VerifyWord(Word) == 1)
          {
            alegereWordNou(1);
          }else if(VerifyWord(Word) == -1)
          {
            setRGB(100, 0, 0);
          }else
            setRGB(0, 100, 0);
      }
      if(millis() - WordSelectionMoment > TypingTime[difficulty])
      {
        alegereWordNou(0);
      }
    }
  }
}