#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

const int LCD_LIGHT_ADDRESS = 100;
const int MTX_LIGHT_ADDRESS = 300;
const int SOUND_ADDRESS = 400;
const int HIGHSCORE_ADDRESS = 800;
const int PLAYERS_ADDRESS = 900;

const int NOTE = 880;
const int NOTE_MOVE = 400;
const int NOTE_NOPE = 300;

const uint64_t setIcon = 0xc0e070381f0d090c;
const uint64_t highscoreIcon = 0x40c0d824241a0207;
const uint64_t aboutIcon = 0x003c181818183c00;
const uint64_t howtoplayIcon = 0x080008081822221c;

long foodTime;
long blinking;
bool foodState = true;

byte NONE = 0;
byte PRESSED = 1;
byte UP = 2;
byte DOWN = 3;
byte LEFT = 4;
byte RIGHT = 5;
byte DIRECTION = 5;

byte best_position = 5;
byte best_score = -1;

struct
{
    String name;
    int score;
} highscore_struct[5];

const byte INTRO = 0;
const byte GAME_MENU = 1;
const byte HIGHSCORE = 2;
const byte SETTINGS = 3;
const byte ABOUT = 4;
const byte HOWTOPLAY =5;
const byte PLAY_GAME =6;
const byte PLAYER =7;
const byte RESET =8;
const byte ENTER_NAME =9;
const byte DIFFICULTY =10;
const byte LCD_CONTRAST =11;
const byte LCD_LIGHT =12;
const byte MATRIX_LIGHT =13;
const byte SOUND =14;
const byte GAME_NAME =15;
const byte GITHUB =16;
const byte MADE_BY =17;
const byte RESET_SETTINGS =18;
const byte GAME_OVER_1 = 19;
const byte GAME_OVER_2 = 20;

bool blocked = false;

long DIFF_1_TIME = 35000;

byte currentMenu = INTRO;
byte gameMenuOptionIndex = 0;
byte highscoreOptionIndex = 0;
byte settingsOptionIndex = 0;
byte aboutOptionIndex = 0;
bool reset = false;
bool reset_settings = false;
byte audio = 0;

long period = 180;
long startTime;

long now_2;
long nowScroll;
long scrollTime = 200;
byte index_scroll = 0;

byte scrollRow;
bool scroll = false;
bool _scroll_ = false;
String scrollOption;

byte indexEnterName = 0;
String setPlayerName = "AAAAAA";
int score = 0;
long played_time;

byte difficulty = 1;
byte _diff_;
byte lcd_light = 1;
byte matrix_light = 1;

byte coord[64][2] = {{-1,-1}};
byte snakeLength = 2;

bool CALIBRATED = true;

bool was_here = false;

byte fullChar[8] =
{
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
};

byte emptyChar[8] =
{
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111
};

byte arrow[8] =
{
    B10000,
    B11000,
    B11100,
    B11110,
    B11110,
    B11100,
    B11000,
    B10000
};

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 13;
const byte d7 = 4;
const byte switchPin = 2;
const byte LCDbrightnessPin = 5;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int XaxisPin = A1;
const int YaxisPin = A0;
const byte buzzerPin = 3;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

class Joystick
{

private:
    int xValue;
    int yValue;
    static const int minThreshold = 300;
    static const int maxThreshold = 700;
    static const long debounceDelay = 50;
    bool moveX;
    bool moveY;
    byte lastSwitchState;
    long lastDebounceTime;
    byte lastSteadyState;

public:

    Joystick(const int XaxisPin, const int YaxisPin, const byte switchPin)
    {
        pinMode(XaxisPin, INPUT);
        pinMode(YaxisPin, INPUT);
        pinMode(switchPin, INPUT_PULLUP);
        moveX = true;
        moveY = true;
        lastSteadyState = HIGH;
        lastSwitchState = LOW;
        lastDebounceTime = 0;
    }

    bool move_UP(bool calibrated = true)
    {

        xValue = analogRead(XaxisPin);

        if( calibrated == true )
        {
            if( xValue > minThreshold + 100 && xValue < maxThreshold - 100 )
                moveX = true;

            if( moveX == true )
            {

                if( xValue < minThreshold )
                {
                    moveX = false;
                    return true;
                }
            }
            return false;
        }
        else
        {
            if( xValue < minThreshold )
            {
                return true;
            }

            return false;
        }
    }

    bool move_DOWN(bool calibrated = true)
    {

        xValue = analogRead(XaxisPin);

        if ( calibrated == true )
        {

            if( xValue > minThreshold + 100 && xValue < maxThreshold - 100)
                moveX = true;

            if( moveX == true )
            {

                if( xValue > maxThreshold )
                {
                    moveX = false;
                    return true;
                }
            }
            return false;
        }
        else
        {

            if( xValue > maxThreshold )
            {
                return true;
            }
            return false;
        }
    }

    bool move_LEFT(bool calibrated = true)
    {

        yValue = analogRead(YaxisPin);

        if( calibrated == true )
        {

            if( yValue > minThreshold + 100 && yValue < maxThreshold - 100 )
                moveY = true;

            if( moveY == true )
            {

                if( yValue > maxThreshold )
                {
                    moveY = false;
                    return true;
                }
            }
            return false;
        }
        else
        {
            if( yValue > maxThreshold )
            {
                return true;
            }

            return false;
        }
    }

    bool move_RIGHT(bool calibrated = true)
    {

        yValue = analogRead(YaxisPin);

        if( calibrated == true )
        {

            if( yValue > minThreshold + 100 && yValue < maxThreshold - 100 )
                moveY = true;

            if( moveY == true )
            {

                if( yValue < minThreshold )
                {
                    moveY = false;
                    return true;
                }
            }
            return false;
        }
        else
        {
            if( yValue < minThreshold )
            {
                return true;
            }

            return false;
        }
    }

    bool pressButton()
    {

        bool pressed = false;
        byte currentSwitchState = digitalRead(switchPin);

        if( currentSwitchState != lastSwitchState )
        {
            lastDebounceTime = millis();
            lastSwitchState = currentSwitchState;
        }

        if( millis() - lastDebounceTime > debounceDelay )
        {

            if( lastSteadyState == HIGH && currentSwitchState == LOW )
            {
                pressed = true;
            }
            lastSteadyState = currentSwitchState;
        }

        return pressed;

    }

    byte followJoystickMove()
    {

        if(pressButton() == true)
        {
            return PRESSED;
        }

        if(move_UP(CALIBRATED) == true)
        {
            return UP;
        }

        if(move_DOWN(CALIBRATED) == true)
        {
            return DOWN;
        }

        if(move_LEFT(CALIBRATED) == true)
        {
            return LEFT;
        }

        if(move_RIGHT(CALIBRATED) == true)
        {
            return RIGHT;
        }

        return NONE;

    }

} joystick(XaxisPin, YaxisPin, switchPin);

void read_player_names_from_eeprom ()
{

    int i = PLAYERS_ADDRESS;
    byte byte1;
    byte byte2;

    String playerName = "";

    for(; i < 6 + PLAYERS_ADDRESS; i++)
    {
        playerName += char(EEPROM.read(i));
    }

    highscore_struct[0].name = playerName;

    byte1 = EEPROM.read(HIGHSCORE_ADDRESS);
    byte2 = EEPROM.read(HIGHSCORE_ADDRESS + 1);

    highscore_struct[0].score = (byte1 << 8) + byte2;

    playerName = "";

    for(; i < 12 + PLAYERS_ADDRESS; i++)
    {
        playerName += char(EEPROM.read(i));
    }

    highscore_struct[1].name = playerName;

    byte1 = EEPROM.read(HIGHSCORE_ADDRESS + 2);
    byte2 = EEPROM.read(HIGHSCORE_ADDRESS + 3);

    highscore_struct[1].score = (byte1 << 8) + byte2;

    playerName = "";

    for(; i < 18 + PLAYERS_ADDRESS; i++)
    {
        playerName += char(EEPROM.read(i));
    }

    highscore_struct[2].name = playerName;

    byte1 = EEPROM.read(HIGHSCORE_ADDRESS + 4);
    byte2 = EEPROM.read(HIGHSCORE_ADDRESS + 5);

    highscore_struct[2].score = (byte1 << 8) + byte2;

    playerName = "";

    for(; i < 24 + PLAYERS_ADDRESS; i++)
    {
        playerName += char(EEPROM.read(i));
    }

    highscore_struct[3].name = playerName;

    byte1 = EEPROM.read(HIGHSCORE_ADDRESS + 6);
    byte2 = EEPROM.read(HIGHSCORE_ADDRESS + 7);

    highscore_struct[3].score = (byte1 << 8) + byte2;

    playerName = "";

    for(; i < 30 + PLAYERS_ADDRESS; i++)
    {
        playerName += char(EEPROM.read(i));
    }

    highscore_struct[4].name = playerName;

    byte1 = EEPROM.read(HIGHSCORE_ADDRESS + 8);
    byte2 = EEPROM.read(HIGHSCORE_ADDRESS + 9);

    highscore_struct[4].score = (byte1 << 8) + byte2;
}


void update_highscore_in_eeprom()
{

    int i = PLAYERS_ADDRESS;

    for(; i < 6 + PLAYERS_ADDRESS; i++)
    {
        EEPROM.update(i, (highscore_struct[0].name)[i - PLAYERS_ADDRESS]);
    }

    EEPROM.update(HIGHSCORE_ADDRESS, highscore_struct[0].score >> 8);
    EEPROM.update(HIGHSCORE_ADDRESS + 1, highscore_struct[0].score & 0xFF);

    for(; i < 12 + PLAYERS_ADDRESS; i++)
    {
        EEPROM.update(i, (highscore_struct[1].name)[i - PLAYERS_ADDRESS - 6]);
    }

    EEPROM.update(HIGHSCORE_ADDRESS + 2, highscore_struct[1].score >> 8);
    EEPROM.update(HIGHSCORE_ADDRESS + 3, highscore_struct[1].score & 0xFF);

    for(; i < 18 + PLAYERS_ADDRESS; i++)
    {
        EEPROM.update(i, (highscore_struct[2].name)[i - PLAYERS_ADDRESS - 12]);
    }

    EEPROM.update(HIGHSCORE_ADDRESS + 4, highscore_struct[2].score >> 8);
    EEPROM.update(HIGHSCORE_ADDRESS + 5, highscore_struct[2].score & 0xFF);

    for(; i < 24 + PLAYERS_ADDRESS; i++)
    {
        EEPROM.update(i, (highscore_struct[3].name)[i - PLAYERS_ADDRESS - 18]);
    }

    EEPROM.update(HIGHSCORE_ADDRESS + 6, highscore_struct[3].score >> 8);
    EEPROM.update(HIGHSCORE_ADDRESS + 7, highscore_struct[3].score & 0xFF);

    for(; i < 30 + PLAYERS_ADDRESS; i++)
    {
        EEPROM.update(i, (highscore_struct[4].name)[i - PLAYERS_ADDRESS - 24]);
    }

    EEPROM.update(HIGHSCORE_ADDRESS + 8, highscore_struct[4].score >> 8);
    EEPROM.update(HIGHSCORE_ADDRESS + 9, highscore_struct[4].score & 0xFF);

}

void read_lcd_light_from_eeprom()
{

    lcd_light = EEPROM.read(LCD_LIGHT_ADDRESS);

}

void read_matrix_light_from_eeprom()
{

    matrix_light = EEPROM.read(MTX_LIGHT_ADDRESS);

}

void read_sound_from_eeprom()
{
    audio = EEPROM.read(SOUND_ADDRESS);
}

String center(String str)
{
    String toReturn = "";
    for(int i = 0; i < 8 - str.length() / 2; i++)
    {
        toReturn += " ";
    }
    return toReturn + str;
}

void setFirstRow(String str)
{
    lcd.setCursor(0,0);
    lcd.print(str);
}

void setSecondRow(String str)
{
    lcd.setCursor(0,1);
    lcd.print(str);
}

void intro()
{

    lcd.clear();
    setFirstRow( center("LET'S SNAKE!") );
    setSecondRow( center("< PRESS >") );

    blocked = true;

}

void game_menu()
{
    lcd.clear();

    if ( gameMenuOptionIndex == 0 )
    {

        setFirstRow( center("< PLAY GAME >") );
        setSecondRow( center("HIGHSCORE") );
    }
    else if ( gameMenuOptionIndex == 1 )
    {

        setFirstRow( center("PLAY GAME") );
        setSecondRow( center("< HIGHSCORE >") );
    }
    else if ( gameMenuOptionIndex == 2 )
    {

        setFirstRow( center("< SETTINGS >") );
        setSecondRow( center("ABOUT") );
    }
    else if ( gameMenuOptionIndex == 3 )
    {

        setFirstRow( center("SETTINGS") );
        setSecondRow( center("< ABOUT >") );
    }
    else if ( gameMenuOptionIndex == 4 )
    {

        setFirstRow( center("< HOW TO PLAY >") );
    }

    blocked = true;

}

void highscore_menu()
{
    lcd.clear();

    if ( highscoreOptionIndex == 0 )
    {

        setFirstRow( center("1. " + highscore_struct[0].name + " " + String(highscore_struct[0].score, DEC)) );
        setSecondRow( center("2. " + highscore_struct[1].name + " " + String(highscore_struct[1].score, DEC)) );
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
    }
    else if ( highscoreOptionIndex == 1 )
    {
        setFirstRow( center("1. " + highscore_struct[0].name + " " + String(highscore_struct[0].score, DEC)) );
        setSecondRow( center("2. " + highscore_struct[1].name + " " + String(highscore_struct[1].score, DEC)) );
        lcd.setCursor(0, 1);
        lcd.write(byte(2));

    }
    else if ( highscoreOptionIndex == 2 )
    {

        setFirstRow( center("3. " + highscore_struct[2].name + " " + String(highscore_struct[2].score, DEC)) );
        setSecondRow( center("4. " + highscore_struct[3].name + " " + String(highscore_struct[3].score, DEC)) );
        lcd.setCursor(0, 0);
        lcd.write(byte(2));

    }
    else if ( highscoreOptionIndex == 3 )
    {

        setFirstRow( center("3. " + highscore_struct[2].name + " " + String(highscore_struct[2].score, DEC)) );
        setSecondRow( center("4. " + highscore_struct[3].name + " " + String(highscore_struct[3].score, DEC)) );
        lcd.setCursor(0, 1);
        lcd.write(byte(2));

    }
    else if ( highscoreOptionIndex == 4 )
    {

        setFirstRow( center("5. " + highscore_struct[4].name + " " + String(highscore_struct[4].score, DEC)) );
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        setSecondRow( center("RESET") );

    }
    else if ( highscoreOptionIndex == 5 )
    {

        setFirstRow( center("5. " + highscore_struct[4].name + " " + String(highscore_struct[4].score, DEC)) );
        setSecondRow( center("< RESET >") );

    }
    else if ( highscoreOptionIndex == 6 )
    {

        setFirstRow( center("< BACK >") );

    }


    blocked = true;

}

void reset_menu()
{

    lcd.clear();

    setFirstRow( center( "Are you sure?" ) );

    if ( reset == false )
    {

        setSecondRow( center( "< NO >  YES" ) );
    }
    else
    {

        setSecondRow( center( "NO  < YES >" ) );
    }


    blocked = true;

}

void reset_settings_menu()
{

    lcd.clear();

    setFirstRow( center( "Are you sure?" ) );

    if ( reset_settings == false )
    {

        setSecondRow( center( "< NO >  YES" ) );
    }
    else
    {

        setSecondRow( center( "NO  < YES >" ) );
    }



    blocked = true;
}

void settings_menu()
{

    lcd.clear();

    if ( settingsOptionIndex == 0 )
    {

        setFirstRow( center("< Enter name >") );
        setSecondRow( center("Difficulty") );
    }
    else if ( settingsOptionIndex == 1 )
    {

        setFirstRow( center("Enter name") );
        setSecondRow( center("< Difficulty >") );
    }
    else if ( settingsOptionIndex == 2 )
    {

        setFirstRow( center("< LCD contrast >") );
        setSecondRow( center("LCD light") );
    }
    else if ( settingsOptionIndex == 3 )
    {

        setFirstRow( center("LCD contrast") );
        setSecondRow( center("< LCD light >") );
    }
    else if ( settingsOptionIndex == 4 )
    {

        setFirstRow( center("< Matrix light >") );
        setSecondRow( center("Sound") );
    }
    else if ( settingsOptionIndex == 5 )
    {

        setFirstRow( center("Matrix light") );
        setSecondRow( center("< Sound >") );
    }
    else if ( settingsOptionIndex == 6 )
    {

        setFirstRow( center("< Reset >") );
        setSecondRow( center("BACK") );
    }
    else if ( settingsOptionIndex == 7 )
    {

        setFirstRow( center("Reset") );
        setSecondRow( center("< BACK >") );
    }


    blocked = true;

}

void enter_name()
{

    lcd.clear();

    setFirstRow( center("Name: " + setPlayerName) );
    if (was_here == false )
    {

        setSecondRow( center("< BACK >") );
    }
    else
    {
        setSecondRow( center("PRESS TO START") );
    }

 

    lcd.setCursor(5, 0);
    indexEnterName = 0;
    lcd.cursor();

    blocked = true;
}

void set_difficulty()
{

    lcd.clear();

    setFirstRow( center( "- #### +" ) );

    lcd.setCursor(6, 0);
    for( int i = 0; i < difficulty; i++)
    {
        lcd.write(byte(1));
    }

    for( int i = difficulty; i < 4; i++)
    {
        lcd.write(byte(0));
    }

    setSecondRow( center( "< BACK >" ) );
    lcd.setCursor(7, 0);

    blocked = true;

}

void lcd_contrast()
{

    lcd.clear();

    setSecondRow( center("< BACK >") );



    scrollRow = 0;
    scrollOption = "   Use potentiometer for LCD contrast";

    nowScroll = millis();
    scroll = true;



    blocked = true;
}

void set_lcd_light()
{

    lcd.clear();

    setFirstRow( center( "- ###### +" ) );

    lcd.setCursor(5, 0);
    for( int i = 0; i < lcd_light; i++)
    {
        lcd.write(byte(1));
    }

    for( int i = lcd_light; i < 6; i++)
    {
        lcd.write(byte(0));
    }

    setSecondRow( center( "< BACK >" ) );
    lcd.setCursor(6, 0);


    blocked = true;

}

void set_matrix_light()
{

    lcd.clear();

    setFirstRow( center( "- ###### +" ) );

    lcd.setCursor(5, 0);
    for( int i = 0; i < matrix_light; i++)
    {
        lcd.write(byte(1));
    }

    for( int i = matrix_light; i < 6; i++)
    {
        lcd.write(byte(0));
    }

    setSecondRow( center( "< BACK >" ) );
    lcd.setCursor(6, 0);


    blocked = true;

}

void set_sound()
{
    lcd.clear();

    setFirstRow( center( "Set sound" ) );

    if(audio == 1)
    {
        setSecondRow( center( "< ON >   OFF" ) );

    }
    else
    {
        setSecondRow( center( "ON   < OFF >" ) );

    }


    blocked = true;
}

void about_menu()
{

    lcd.clear();

    if ( aboutOptionIndex == 0 )
    {

        setFirstRow( center("< Game name >") );
        setSecondRow( center("Github") );
    }
    else if ( aboutOptionIndex == 1 )
    {

        setFirstRow( center("Game name") );
        setSecondRow( center("< Github >") );
    }
    else if ( aboutOptionIndex == 2 )
    {

        setFirstRow( center("< Made by >") );
        setSecondRow( center("BACK") );
    }
    else if ( aboutOptionIndex == 3 )
    {

        setFirstRow( center("Made by") );
        setSecondRow( center("< BACK >") );
    }


    blocked = true;
}

void game_name()
{

    lcd.clear();

    setFirstRow( center("SNAKE") );
    setSecondRow( center("< BACK >") );



    blocked = true;

}

void github()
{

    lcd.clear();

    setFirstRow( center("Dani780-C") );
    setSecondRow( center("< BACK >") );



    blocked = true;

}

void made_by()
{

    lcd.clear();

    setFirstRow( center("Cirlan Daniel") );
    setSecondRow( center("< BACK >") );



    blocked = true;

}

void howtoplay()
{

    lcd.clear();

    setSecondRow(center("< BACK >"));



    lcd.setCursor(0, 0);

    scrollRow = 0;
    scrollOption = "   Use the joystick to eat as much food as possible on the matrix.";

    nowScroll = millis();
    scroll = true;


    blocked = true;

}

void game_info()
{
    lcd.clear();

    setFirstRow( " " + setPlayerName + " Time " + String(played_time) );
    setSecondRow("Dif " + String(difficulty) + " Score " + String(score) );

    played_time = DIFF_1_TIME;


    scroll = false;
    blocked = true;


}

long now;

void update_info()
{

    lcd.setCursor(4,0);

    lcd.setCursor(13, 0);

    if( millis() - now > 1000 )
    {
        now = millis();
        played_time -= 1000;
        if( played_time / 10000 < 10 )
        {
            lcd.setCursor(13, 0);
            lcd.write("   ");
        }
    }

    lcd.setCursor(13, 0);
    lcd.print(played_time / 1000);

    lcd.setCursor(4, 1);

    lcd.print(difficulty);

    lcd.setCursor(12,1);

    lcd.print(score);

}

bool beat;

void sortare_highscore()
{
    byte n = 5;
    bool contor = false;
    for (int i = 1; i < n; ++i)
    {
        int key = highscore_struct[i].score;
        String playerName = highscore_struct[i].name;
        if ( i == best_position )
            contor = true;

        byte j = i - 1;
        while (j >= 0 && highscore_struct[j].score < key)
        {
            highscore_struct[j + 1].score = highscore_struct[j].score;
            highscore_struct[j + 1].name = highscore_struct[j].name;

            j = j - 1;
        }
        if ( contor == true )
        {
            best_position = j + 1;
            contor = false;
        }
        highscore_struct[j + 1].score = key;
        highscore_struct[j + 1].name = playerName;
    }
    
}

void game_over_1()
{

    beat = false;

    if ( best_position == 5 )
    {
        highscore_struct[4].name = setPlayerName;
        highscore_struct[4].score = score;
        best_score = score;
        best_position = 4;
        sortare_highscore();
        update_highscore_in_eeprom();
    }
    else
    {

        if ( highscore_struct[best_position].score < score )
        {
            highscore_struct[best_position].score = score;
            sortare_highscore();
            update_highscore_in_eeprom();
        }

    }

    if ( best_position == 0)
        beat = true;

    lcd.clear();

    scrollOption = "    Well done for the score of " + String(score) + " points.";
    scroll = true;

    setSecondRow( center("< PRESS >") );

    blocked = true;

}

void game_over_2()
{

    lcd.clear();

    scrollOption = "    Name: " + setPlayerName + " Score: " + String(score);

    if ( beat == true )
    {
        scrollOption += " YOU BEAT THE HIGHSCORE!";
    }

    setSecondRow( center( "< PRESS >" ) );

    scroll = true;
    blocked = true;

}

void reset_settings_function()
{

    EEPROM.update(LCD_LIGHT_ADDRESS, 3);
    lcd_light = 3;
    analogWrite(LCDbrightnessPin, lcd_light * 40);

    EEPROM.update(MTX_LIGHT_ADDRESS, 3);
    matrix_light = 3;
    lc.setIntensity(0, matrix_light * 2);

    EEPROM.update(SOUND_ADDRESS, 1);
    audio = 1;

    setPlayerName = "AAAAAA";
    was_here = false;
    difficulty = 1;

}

bool changed = true;

byte switchMenu()
{

    blocked = false;

    scroll = false;
    index_scroll = 0;
    changed = true;

    if( currentMenu == INTRO )
    {
        return GAME_MENU;
    }
    if( currentMenu == GAME_MENU && gameMenuOptionIndex == 0 )
    {
        set_snake();
        return PLAY_GAME;
    }
    if( currentMenu == GAME_MENU && gameMenuOptionIndex == 1 )
    {
        return HIGHSCORE;
    }
    if( currentMenu == GAME_MENU && gameMenuOptionIndex == 2 )
    {
        return SETTINGS;
    }
    if( currentMenu == GAME_MENU && gameMenuOptionIndex == 3 )
    {
        return ABOUT;
    }
    if( currentMenu == GAME_MENU && gameMenuOptionIndex == 4 )
    {
        return HOWTOPLAY;
    }

    if( currentMenu == HIGHSCORE && highscoreOptionIndex == 5 )
    {
        return RESET;
    }
    if( currentMenu == HIGHSCORE && highscoreOptionIndex == 6 )
    {
        highscoreOptionIndex = 0;
        return GAME_MENU;
    }

    if ( currentMenu == RESET && reset == false )
    {
        return HIGHSCORE;
    }

    if ( currentMenu == RESET && reset == true )
    {

        for( byte i = 0; i < 5; i++)
        {
            highscore_struct[i].name = " N/A  ";
            highscore_struct[i].score = 0;
        }

        update_highscore_in_eeprom();
        reset = false;
        best_score = -1;
        best_position = 5;
        return HIGHSCORE;
    }

    if ( currentMenu == SETTINGS && settingsOptionIndex == 0)
    {
        was_here = false;
        return ENTER_NAME;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 1 )
    {
        return DIFFICULTY;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 2 )
    {
        return LCD_CONTRAST;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 3 )
    {
        return LCD_LIGHT;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 4 )
    {
        return MATRIX_LIGHT;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 5 )
    {
        return SOUND;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 6 )
    {
        return RESET_SETTINGS;
    }
    if ( currentMenu == SETTINGS && settingsOptionIndex == 7 )
    {
        settingsOptionIndex = 0;
        return GAME_MENU;
    }

    if ( currentMenu == ENTER_NAME && was_here == true)
    {
        scroll = false;
        lcd.noCursor();
        return PLAY_GAME;
    }

    if ( currentMenu == ENTER_NAME && was_here == false)
    {
        if ( best_position < 5 )
        {
            highscore_struct[best_position].name = setPlayerName;
            update_highscore_in_eeprom();
        }
        was_here = true;
        lcd.noCursor();
        return SETTINGS;
    }
    if ( currentMenu == DIFFICULTY )
    {
        return SETTINGS;
    }
    if ( currentMenu == LCD_CONTRAST )
    {
        return SETTINGS;
    }
    if ( currentMenu == LCD_LIGHT )
    {
        return SETTINGS;
    }
    if( currentMenu == MATRIX_LIGHT )
    {
        return SETTINGS;
    }
    if ( currentMenu == SOUND )
    {
        return SETTINGS;
    }

    if ( currentMenu == ABOUT && aboutOptionIndex == 0)
    {
        return GAME_NAME;
    }
    if ( currentMenu == ABOUT && aboutOptionIndex == 1)
    {
        return GITHUB;
    }
    if ( currentMenu == ABOUT && aboutOptionIndex == 2)
    {
        return MADE_BY;
    }
    if ( currentMenu == ABOUT && aboutOptionIndex == 3)
    {
        aboutOptionIndex = 0;
        return GAME_MENU;
    }

    if ( currentMenu == GAME_NAME )
    {
        return ABOUT;
    }
    if ( currentMenu == GITHUB )
    {
        return ABOUT;
    }
    if ( currentMenu == MADE_BY )
    {
        return ABOUT;
    }
    if(currentMenu == HOWTOPLAY )
    {
        return GAME_MENU;
    }
    if ( currentMenu == RESET_SETTINGS )
    {
        if (reset_settings == true )
        {
            reset_settings_function();
            reset_settings = false;
        }
        return SETTINGS;
    }

    if ( currentMenu == GAME_OVER_1 )
    {
        return GAME_OVER_2;
    }

    if ( currentMenu == GAME_OVER_2 )
    {
        set_snake();
        score = 0;
        CALIBRATED = true;
        return GAME_MENU;
    }

    return currentMenu;

}

bool eaten = true;
byte row, col;


bool collide()
{


    for ( int i = 1; i < snakeLength; i++)
    {
        if ( coord[0][0] == coord[i][0] && coord[0][1] == coord[i][1])
        {

            return true;

        }
    }

    if( difficulty == 3 )
    {
        if ( ( coord[0][0] == 1 && coord[0][1] == 3 ) ||
                ( coord[0][0] == 1 && coord[0][1] == 4) ||
                ( coord[0][0] == 1 && coord[0][1] == 5) ||
                ( coord[0][0] == 2 && coord[0][1] == 5) ||
                ( coord[0][0] == 6 && coord[0][1] == 4) ||
                ( coord[0][0] == 6 && coord[0][1] == 5) ||
                ( coord[0][0] == 7 && coord[0][1] == 4) ||
                ( coord[0][0] == 7 && coord[0][1] == 5) )
            return true;
    }
    else if ( difficulty == 4)
    {

        if ( ( coord[0][0] == 5 && coord[0][1] == 2 ) ||
                ( coord[0][0] == 5 && coord[0][1] == 3) ||
                ( coord[0][0] == 6 && coord[0][1] == 2) ||
                ( coord[0][0] == 6 && coord[0][1] == 3) ||
                ( coord[0][0] == 1 && coord[0][1] == 6) ||
                ( coord[0][0] == 2 && coord[0][1] == 6) ||
                ( coord[0][0] == 4 && coord[0][1] == 6) ||
                ( coord[0][0] == 5 && coord[0][1] == 6) )
            return true;
    }

    return false;

}


bool inSnake()
{

    for ( int i = 0; i < snakeLength; i++)
    {
        if ( row == coord[i][0] && col == coord[i][1])
        {

            return true;

        }
    }

    return false;

}


void set_snake()
{

    for( int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
            lc.setLed(0, i, j, 0);
    }

    snakeLength = 2;
    eaten = true;
    DIRECTION = RIGHT;

    coord[0][0] = 3;
    coord[0][1] = 2;
    coord[1][0] = 3;
    coord[1][1] = 1;

    for(int i = 0; i < snakeLength; i++)
    {
        lc.setLed(0, coord[i][0], coord[i][1], 1);
    }

    if ( difficulty == 3 )
    {
        lc.setLed(0, 1, 3, 1);
        lc.setLed(0, 1, 4, 1);
        lc.setLed(0, 1, 5, 1);
        lc.setLed(0, 2, 5, 1);

        lc.setLed(0, 6, 4, 1);
        lc.setLed(0, 6, 5, 1);
        lc.setLed(0, 7, 4, 1);
        lc.setLed(0, 7, 5, 1);
    }
    else if ( difficulty == 4 )
    {
        lc.setLed(0, 5, 2, 1);
        lc.setLed(0, 5, 3, 1);
        lc.setLed(0, 6, 2, 1);
        lc.setLed(0, 6, 3, 1);

        lc.setLed(0, 1, 6, 1);
        lc.setLed(0, 2, 6, 1);
        lc.setLed(0, 4, 6, 1);
        lc.setLed(0, 5, 6, 1);
    }


}

void game(byte joystickMove)
{

    switch(difficulty)
    {

    case 1:

        if( millis() < startTime + DIFF_1_TIME )
        {

            if( millis() > now_2 + period)
            {

                byte r = coord[0][0];
                byte c = coord[0][1];

                if ( joystickMove == DOWN && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == DOWN && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == RIGHT )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == RIGHT )
                    joystickMove = NONE;

                if( joystickMove == UP )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                        {
                            coord[0][0] = 7;
                        }

                        DIRECTION = UP;

                    }

                }
                else if( joystickMove == DOWN )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] < 7)
                        {
                            coord[0][0]++;
                        }
                        else
                        {
                            coord[0][0] = 0;
                        }

                        DIRECTION = DOWN;

                    }

                }
                else if( joystickMove == LEFT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                        {
                            coord[0][1] = 7;
                        }

                        DIRECTION = LEFT;

                    }

                }
                else if( joystickMove == RIGHT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                        {
                            coord[0][1] = 0;
                        }

                        DIRECTION = RIGHT;

                    }

                }
                else
                {
                    if ( DIRECTION == LEFT )
                    {
                        if ( coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                            coord[0][1] = 7;
                    }
                    else if ( DIRECTION == RIGHT )
                    {
                        if ( coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                            coord[0][1] = 0;
                    }
                    else if ( DIRECTION == UP )
                    {
                        if ( coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                            coord[0][0] = 7;
                    }
                    else if ( DIRECTION == DOWN )
                    {
                        if ( coord[0][0] < 7 )
                        {
                            coord[0][0]++;
                        }
                        else
                            coord[0][0] = 0;
                    }

                }

                lc.setLed(0, coord[0][0], coord[0][1], 1);

                if ( collide() == true )
                {

                    currentMenu = GAME_OVER_1;
                    blocked = false;
                    difficulty = _diff_;

                }
                else
                {

                    if ( eaten == true )
                    {

                        row = rand() % 8;
                        col = rand() % 8;

                        lc.setLed(0, row, col, 1);

                        eaten = false;

                    }

                    if ( coord[0][0] == row && coord[0][1] == col )
                    {

                        eaten = true;
                        snakeLength += 1;

                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;

                        coord[0][0] = row;
                        coord[0][1] = col;

                        if ( audio == 1 )
                        {
                            period = 50;
                            tone(buzzerPin, audio * 880, 150);
                        }
                        else
                            period = 70;

                        score += 1;

                    }
                    else
                    {

                        period = 180;

                        lc.setLed(0, coord[snakeLength - 1][0], coord[snakeLength - 1][1], 0);

                        if ( eaten == false )
                            lc.setLed(0, row, col, 1);



                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;


                    }

                }

                now_2 = millis();
            }


        }
        else
        {
            difficulty += 1;
            played_time = DIFF_1_TIME;
            startTime = millis();
            period = 180;
            set_snake();
        }

        break;

    case 2:

        if( millis() < startTime + DIFF_1_TIME )
        {

            if( millis() > now_2 + period)
            {

                byte r = coord[0][0];
                byte c = coord[0][1];

                if ( joystickMove == DOWN && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == DOWN && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == RIGHT )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == RIGHT )
                    joystickMove = NONE;


                if( joystickMove == UP )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                        {
                            coord[0][0] = 7;
                        }

                        DIRECTION = UP;

                    }

                }
                else if( joystickMove == DOWN )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] < 7)
                        {
                            coord[0][0]++;
                        }
                        else
                        {
                            coord[0][0] = 0;
                        }

                        DIRECTION = DOWN;

                    }

                }
                else if( joystickMove == LEFT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                        {
                            coord[0][1] = 7;
                        }

                        DIRECTION = LEFT;

                    }

                }
                else if( joystickMove == RIGHT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                        {
                            coord[0][1] = 0;
                        }

                        DIRECTION = RIGHT;

                    }

                }
                else
                {
                    if ( DIRECTION == LEFT )
                    {
                        if ( coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                            coord[0][1] = 7;
                    }
                    else if ( DIRECTION == RIGHT )
                    {
                        if ( coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                            coord[0][1] = 0;
                    }
                    else if ( DIRECTION == UP )
                    {
                        if ( coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                            coord[0][0] = 7;
                    }
                    else if ( DIRECTION == DOWN )
                    {
                        if ( coord[0][0] < 7 )
                        {
                            coord[0][0]++;
                        }
                        else
                            coord[0][0] = 0;
                    }

                }

                lc.setLed(0, coord[0][0], coord[0][1], 1);

                if ( collide() == true )
                {

                    currentMenu = GAME_OVER_1;
                    difficulty = _diff_;
                    blocked = false;

                }
                else
                {

                    if ( eaten == true )
                    {

                        row = rand() % 8;
                        col = rand() % 8;

                        lc.setLed(0, row, col, 1);

                        eaten = false;

                        foodTime = millis();

                    }

                    if ( millis() > foodTime + 1900 )
                    {
                        if ( inSnake() == false )
                            lc.setLed(0, row, col, 0);
                        eaten = true;
                    }

                    if ( coord[0][0] == row && coord[0][1] == col && eaten == false )
                    {

                        eaten = true;
                        snakeLength += 1;

                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }


                        coord[1][0] = r;
                        coord[1][1] = c;

                        coord[0][0] = row;
                        coord[0][1] = col;

                        if ( audio == 1 )
                        {
                            period = 50;
                            tone(buzzerPin, audio * 880, 150);
                        }
                        else
                            period = 70;

                        score += 2;

                    }
                    else
                    {

                        period = 180;

                        lc.setLed(0, coord[snakeLength - 1][0], coord[snakeLength - 1][1], 0);

                        if ( eaten == false )
                            lc.setLed(0, row, col, 1);



                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;


                    }

                }

                now_2 = millis();
            }


        }
        else
        {
            difficulty += 1;
            played_time = DIFF_1_TIME;
            startTime = millis();
            period = 180;

            set_snake();
        }

        break;

    case 3:

        if( millis() < startTime + DIFF_1_TIME )
        {

            if( millis() > now_2 + period)
            {

                byte r = coord[0][0];
                byte c = coord[0][1];

                if ( joystickMove == DOWN && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == DOWN && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == RIGHT )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == RIGHT )
                    joystickMove = NONE;


                if( joystickMove == UP )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                        {
                            coord[0][0] = 7;
                        }

                        DIRECTION = UP;

                    }

                }
                else if( joystickMove == DOWN )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] < 7)
                        {
                            coord[0][0]++;
                        }
                        else
                        {
                            coord[0][0] = 0;
                        }

                        DIRECTION = DOWN;

                    }

                }
                else if( joystickMove == LEFT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                        {
                            coord[0][1] = 7;
                        }

                        DIRECTION = LEFT;

                    }

                }
                else if( joystickMove == RIGHT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                        {
                            coord[0][1] = 0;
                        }

                        DIRECTION = RIGHT;

                    }

                }
                else
                {
                    if ( DIRECTION == LEFT )
                    {
                        if ( coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                            coord[0][1] = 7;
                    }
                    else if ( DIRECTION == RIGHT )
                    {
                        if ( coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                            coord[0][1] = 0;
                    }
                    else if ( DIRECTION == UP )
                    {
                        if ( coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                            coord[0][0] = 7;
                    }
                    else if ( DIRECTION == DOWN )
                    {
                        if ( coord[0][0] < 7 )
                        {
                            coord[0][0]++;
                        }
                        else
                            coord[0][0] = 0;
                    }

                }

                lc.setLed(0, coord[0][0], coord[0][1], 1);

                if ( collide() == true )
                {

                    currentMenu = GAME_OVER_1;
                    difficulty = _diff_;
                    blocked = false;

                }
                else
                {

                    if ( eaten == true )
                    {
                        byte row_val[11][2] = {{ 0, 0}, {4, 7}, {6, 0}, {0, 5}, {7, 7}, {2, 2}, {2, 3}, {0, 7}, {5, 4}, {4, 4}, {6, 6}};

                        byte rand = random(0, 1000) % 11;
                        row = row_val[rand][0];
                        col = row_val[rand][1];

                        lc.setLed(0, row, col, 1);

                        eaten = false;
                        blinking = millis();
                        foodState = true;

                    }

                    if ( millis() > blinking  + 300 )
                    {
                        blinking = millis();
                        if( inSnake() == false )
                        {
                            lc.setLed(0, row, col, foodState);
                            foodState = !foodState;
                        }
                    }

                    if ( coord[0][0] == row && coord[0][1] == col )
                    {

                        eaten = true;
                        snakeLength += 1;

                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;

                        coord[0][0] = row;
                        coord[0][1] = col;

                        if ( audio == 1 )
                        {
                            period = 50;
                            tone(buzzerPin, audio * 880, 150);
                        }
                        else
                            period = 70;

                        score += 3;

                    }
                    else
                    {

                        period = 180;


                        lc.setLed(0, coord[snakeLength - 1][0], coord[snakeLength - 1][1], 0);

                        if ( eaten == false )
                            lc.setLed(0, row, col, foodState);



                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;


                    }

                }

                now_2 = millis();
            }


        }
        else
        {
            difficulty += 1;
            played_time = DIFF_1_TIME;
            startTime = millis();
            period = 180;
            set_snake();
        }

        break;

    case 4:

        if( millis() < startTime + DIFF_1_TIME )
        {

            if( millis() > now_2 + period)
            {

                byte r = coord[0][0];
                byte c = coord[0][1];

                if ( joystickMove == DOWN && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == DOWN )
                    joystickMove = NONE;
                else if ( joystickMove == UP && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == DOWN && DIRECTION == UP )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == LEFT )
                    joystickMove = NONE;
                else if ( joystickMove == RIGHT && DIRECTION == RIGHT )
                    joystickMove = NONE;
                else if ( joystickMove == LEFT && DIRECTION == RIGHT )
                    joystickMove = NONE;


                if( joystickMove == UP )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                        {
                            coord[0][0] = 7;
                        }

                        DIRECTION = UP;

                    }

                }
                else if( joystickMove == DOWN )
                {

                    if( DIRECTION == LEFT || DIRECTION == RIGHT)
                    {

                        if(coord[0][0] < 7)
                        {
                            coord[0][0]++;
                        }
                        else
                        {
                            coord[0][0] = 0;
                        }

                        DIRECTION = DOWN;

                    }

                }
                else if( joystickMove == LEFT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                        {
                            coord[0][1] = 7;
                        }

                        DIRECTION = LEFT;

                    }

                }
                else if( joystickMove == RIGHT )
                {

                    if( DIRECTION == UP || DIRECTION == DOWN)
                    {

                        if(coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                        {
                            coord[0][1] = 0;
                        }

                        DIRECTION = RIGHT;

                    }

                }
                else
                {
                    if ( DIRECTION == LEFT )
                    {
                        if ( coord[0][1] > 0)
                        {
                            coord[0][1]--;
                        }
                        else
                            coord[0][1] = 7;
                    }
                    else if ( DIRECTION == RIGHT )
                    {
                        if ( coord[0][1] < 7)
                        {
                            coord[0][1]++;
                        }
                        else
                            coord[0][1] = 0;
                    }
                    else if ( DIRECTION == UP )
                    {
                        if ( coord[0][0] > 0)
                        {
                            coord[0][0]--;
                        }
                        else
                            coord[0][0] = 7;
                    }
                    else if ( DIRECTION == DOWN )
                    {
                        if ( coord[0][0] < 7 )
                        {
                            coord[0][0]++;
                        }
                        else
                            coord[0][0] = 0;
                    }

                }

                lc.setLed(0, coord[0][0], coord[0][1], 1);

                if ( collide() == true )
                {

                    currentMenu = GAME_OVER_1;
                    difficulty = _diff_;
                    blocked = false;

                }
                else
                {

                    if ( eaten == true )
                    {

                        byte row_val[11][2] = {{ 0, 0}, {3, 3}, {6, 0}, {0, 5}, {7, 7}, {6, 6}, {6, 5}, {5, 5}, {6, 4}, {4, 4}, {3, 7}};

                        byte rand = random(0, 1000) % 11;
                        row = row_val[rand][0];
                        col = row_val[rand][1];

                        lc.setLed(0, row, col, 1);

                        foodTime = millis();
                        eaten = false;

                        blinking = millis();
                        foodState = true;

                    }

                    if ( millis() > foodTime + 2200 )
                    {
                        if ( inSnake() == false )
                            lc.setLed(0, row, col, 0);
                        eaten = true;
                    }

                    if ( millis() > blinking  + 300 )
                    {
                        blinking = millis();
                        if( inSnake() == false )
                        {
                            lc.setLed(0, row, col, foodState);
                            foodState = !foodState;
                        }
                    }

                    if ( coord[0][0] == row && coord[0][1] == col )
                    {

                        eaten = true;
                        snakeLength += 1;

                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }
                        coord[1][0] = r;
                        coord[1][1] = c;

                        coord[0][0] = row;
                        coord[0][1] = col;

                        if ( audio == 1 )
                        {
                            period = 50;
                            tone(buzzerPin, audio * 880, 150);
                        }
                        else
                            period = 70;

                        score += 4;

                    }
                    else
                    {

                        period = 180;

                        lc.setLed(0, coord[snakeLength - 1][0], coord[snakeLength - 1][1], 0);

                        if ( inSnake() == false && eaten == false )
                            lc.setLed(0, row, col, foodState);
                        else if ( inSnake() == true && eaten == true)
                        {
                            lc.setLed(0, row, col, 1);
                        }



                        for(int i = snakeLength - 1; i > 1; i--)
                        {
                            coord[i][0] = coord[i-1][0];
                            coord[i][1] = coord[i-1][1];
                        }

                        coord[1][0] = r;
                        coord[1][1] = c;


                    }

                }

                now_2 = millis();
            }


        }
        else
        {

            currentMenu = GAME_OVER_1;
            blocked = false;
            set_snake();

            period = 180;

            CALIBRATED = true;

            difficulty = _diff_;
        }

        break;

    }

}


void scrollFunction(String option, byte scrollRow, bool _scroll )
{

    if ( _scroll == true )
    {
        lcd.setCursor(0, scrollRow);

        if( millis() > nowScroll + scrollTime)
        {
            if ( _scroll_ == true)
            {

                if ( index_scroll == option.length() ) index_scroll = 0;

                String newOp = "";

                for(int i = index_scroll; i < 16 + index_scroll && i < option.length(); i++)
                    newOp += option[i];

                byte a = 16 - newOp.length();
                for (int i = 0; i < a ; i++)
                    newOp += " ";

                lcd.print( newOp );
                _scroll_ = false;
                index_scroll++;
            }
            else
            {
                _scroll_ = true;
            }
            nowScroll = millis();
        }
    }
}

void setup()
{

    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    lcd.begin(16, 2);
    lcd.createChar(1, fullChar);
    lcd.createChar(0, emptyChar);
    lcd.createChar(2, arrow);
    pinMode(LCDbrightnessPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    lc.shutdown(0, false);
    lc.clearDisplay(0);
    read_player_names_from_eeprom();
    read_lcd_light_from_eeprom();
    read_matrix_light_from_eeprom();
    read_sound_from_eeprom();
    analogWrite(LCDbrightnessPin, lcd_light * 40);
    lc.setIntensity(0, matrix_light * 2);
    randomSeed(63143);
};

void set_animation(const uint64_t anim)
{

    for (int i = 0; i < 8; i++)
    {
        byte r = (anim >> i * 8) & 0xFF;
        for (int j = 0; j < 8; j++)
        {
            lc.setLed(0, i, j, bitRead(r, j));
        }
    }

}


void animation()
{

    if ( changed == true )
    {
        if ( currentMenu == INTRO || currentMenu == GAME_MENU )
        {

            for( byte i = 0; i < 8; i++)
            {
                for(byte j = 0; j < 8; j++)
                {
                    lc.setLed(0, i, j, 0);
                }
            }

            lc.setLed(0, 1, 2, 1);
            lc.setLed(0, 1, 3, 1);
            lc.setLed(0, 1, 4, 1);
            lc.setLed(0, 1, 5, 1);
            lc.setLed(0, 2, 2, 1);
            lc.setLed(0, 3, 2, 1);
            lc.setLed(0, 4, 2, 1);
            lc.setLed(0, 4, 3, 1);
            lc.setLed(0, 4, 4, 1);
            lc.setLed(0, 4, 5, 1);
            lc.setLed(0, 5, 5, 1);
            lc.setLed(0, 6, 5, 1);
            lc.setLed(0, 6, 4, 1);
            lc.setLed(0, 6, 3, 1);
            lc.setLed(0, 6, 2, 1);


        }
        else if ( currentMenu == SETTINGS )
        {
            set_animation(setIcon);
        }
        else if ( currentMenu == HIGHSCORE )
        {
            set_animation(highscoreIcon);
        }
        else if ( currentMenu == ABOUT )
        {
            set_animation(aboutIcon);
        }
        else if (currentMenu == HOWTOPLAY )
        {
            set_animation(howtoplayIcon);

        }
        changed = false;
    }
}

void loop()
{

    byte joystickMove = joystick.followJoystickMove();

    animation();
    scrollFunction(scrollOption, scrollRow, scroll);


    if ( joystickMove == PRESSED && currentMenu != PLAY_GAME)
    {
        tone(buzzerPin, audio * NOTE, 150);
        currentMenu = switchMenu();
    }

    switch(currentMenu)
    {

    case INTRO:

        if ( blocked == false )
            intro();

        break;

    case GAME_MENU:

        if ( joystickMove == UP )
        {

            if ( gameMenuOptionIndex > 0 )
            {
                gameMenuOptionIndex--;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }
        else if ( joystickMove == DOWN )
        {

            if ( gameMenuOptionIndex < 4 )
            {
                gameMenuOptionIndex++;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }

        if ( blocked == false )
            game_menu();

        break;

    case PLAY_GAME:

        if(was_here == false && blocked == false)
        {
            was_here = true;
            currentMenu = ENTER_NAME;
        }

        else if (blocked == false)
        {
            game_info();
            now_2 = period;
            CALIBRATED = false;
            startTime = millis();
            _diff_ = difficulty;
        }
        else
            game(joystickMove);


        update_info();

        break;

    case HIGHSCORE:

        if ( joystickMove == UP )
        {

            if ( highscoreOptionIndex > 0 )
            {
                highscoreOptionIndex--;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }
        else if ( joystickMove == DOWN )
        {

            if ( highscoreOptionIndex < 6 )
            {
                highscoreOptionIndex++;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }

        if ( blocked == false )
            highscore_menu();

        break;

    case RESET:

        if ( joystickMove == LEFT )
        {

            if ( reset == true )
            {
                reset = false;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( reset == false )
            {
                reset = true;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }


        }

        if ( blocked == false )
            reset_menu();

        break;

    case SETTINGS:

        if ( joystickMove == UP )
        {

            if ( settingsOptionIndex > 0 )
            {
                settingsOptionIndex--;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }
        else if ( joystickMove == DOWN )
        {

            if ( settingsOptionIndex < 7 )
            {
                settingsOptionIndex++;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }

        if ( blocked == false )
            settings_menu();

        break;

    case ENTER_NAME:

        lcd.setCursor(8 + indexEnterName, 0);
        lcd.cursor();
        if ( joystickMove == UP )
        {

            if ( setPlayerName[indexEnterName] == 'Z' )
            {
                setPlayerName[indexEnterName] = 'A';
            }
            else
                setPlayerName[indexEnterName] += 1;

            lcd.setCursor(8 + indexEnterName, 0);
            lcd.print(setPlayerName[indexEnterName]);
            lcd.setCursor(8 + indexEnterName, 0);

        }
        else if ( joystickMove == DOWN )
        {

            if ( setPlayerName[indexEnterName] == 'A' )
            {
                setPlayerName[indexEnterName] = 'Z';
            }
            else
                setPlayerName[indexEnterName] -= 1;

            lcd.setCursor(8 + indexEnterName, 0);
            lcd.print(setPlayerName[indexEnterName]);
            lcd.setCursor(8 + indexEnterName, 0);

        }
        else if ( joystickMove == LEFT )
        {

            if ( indexEnterName > 0 )
            {
                indexEnterName--;
                lcd.setCursor(8 + indexEnterName, 0);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( indexEnterName < 5 )
            {
                indexEnterName++;
                lcd.setCursor(8 + indexEnterName, 0);
            }

        }

        if ( blocked == false )
            enter_name();

        break;

    case DIFFICULTY:

        if ( joystickMove == LEFT )
        {

            if ( difficulty > 1 )
            {
                difficulty--;
                lcd.setCursor(6 + difficulty, 0);
                lcd.write(byte(0));
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( difficulty < 4 )
            {
                lcd.setCursor(6 + difficulty, 0);
                difficulty++;
                lcd.write(byte(1));
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }


        }
        if ( blocked == false )
            set_difficulty();

        break;

    case LCD_CONTRAST:

        if( blocked == false )
            lcd_contrast();

        break;

    case LCD_LIGHT:

        if ( joystickMove == LEFT )
        {

            if ( lcd_light > 1 )
            {
                lcd_light--;
                lcd.setCursor(5 + lcd_light, 0);
                lcd.write(byte(0));
                analogWrite(LCDbrightnessPin, lcd_light * 40);
                EEPROM.update(LCD_LIGHT_ADDRESS, lcd_light);
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( lcd_light < 6 )
            {
                lcd.setCursor(5 + lcd_light, 0);
                lcd_light++;
                lcd.write(byte(1));
                analogWrite(LCDbrightnessPin, lcd_light * 40);
                EEPROM.update(LCD_LIGHT_ADDRESS, lcd_light);
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }

        if( blocked == false )
            set_lcd_light();

        break;

    case MATRIX_LIGHT:

        if ( joystickMove == LEFT )
        {

            if ( matrix_light > 1 )
            {
                matrix_light--;
                lcd.setCursor(5 + matrix_light, 0);
                lcd.write(byte(0));
                lc.setIntensity(0, matrix_light * 2);
                EEPROM.update(MTX_LIGHT_ADDRESS, matrix_light);
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( matrix_light < 6 )
            {
                lcd.setCursor(5 + matrix_light, 0);
                matrix_light++;
                lcd.write(byte(1));
                lc.setIntensity(0, matrix_light * 2);
                EEPROM.update(MTX_LIGHT_ADDRESS, matrix_light);
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }

        if( blocked == false )
            set_matrix_light();

        break;

    case SOUND:

        if ( joystickMove == LEFT )
        {

            if( audio == 0 )
            {
                audio = 1;
                EEPROM.update(SOUND_ADDRESS, audio);
                blocked = false;
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( audio == 1 )
            {
                audio = 0;
                EEPROM.update(SOUND_ADDRESS, audio);
                blocked = false;
            }

        }

        if( blocked == false )
            set_sound();

        break;

    case RESET_SETTINGS:

        if ( joystickMove == LEFT )
        {

            if ( reset_settings == true )
            {
                reset_settings = false;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }
        else if ( joystickMove == RIGHT )
        {

            if ( reset_settings == false )
            {
                reset_settings = true;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }

        }

        if ( blocked == false )
            reset_settings_menu();

        break;

    case ABOUT:


        if ( joystickMove == UP )
        {

            if ( aboutOptionIndex > 0 )
            {
                aboutOptionIndex--;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }
        else if ( joystickMove == DOWN )
        {

            if ( aboutOptionIndex < 3 )
            {
                aboutOptionIndex++;
                blocked = false;
                tone(buzzerPin, audio * NOTE_MOVE, 150);
            }
            else
            {
                tone(buzzerPin, audio * NOTE_NOPE, 100);
            }

        }

        if ( blocked == false )
            about_menu();

        break;

    case GAME_NAME:

        if( blocked == false )
            game_name();

        break;

    case GITHUB:

        if( blocked == false )
            github();

        break;

    case MADE_BY:

        if( blocked == false )
            made_by();

        break;

    case HOWTOPLAY:

        if( blocked == false )
            howtoplay();

        break;

    case GAME_OVER_1:

        if ( blocked == false )
            game_over_1();

        break;

    case GAME_OVER_2:

        if ( blocked == false )
            game_over_2();

        break;

    default:
        break;
    }


}
