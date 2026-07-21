#include <Arduino.h>
#include "commands.h"
#include "filesystem.h"
#include "shell.h"

void cmd_help()
{
    Serial.println();
    Serial.println("Available Commands");
    Serial.println("------------------");
    Serial.println("help");
    Serial.println("info");
    Serial.println("clear");
    Serial.println("mkdir");
    Serial.println("touch");
    Serial.println("ls");
    Serial.println("cd");
    Serial.println("pwd");
    Serial.println("format");
    Serial.println("tetris");
}

void cmd_info()
{
    Serial.println();
    Serial.println("AJAY OS v0.1.1");
    Serial.println("Processor : ATmega328P");
    Serial.println("Flash     : 32 KB");
    Serial.println("SRAM      : 2 KB");
    Serial.println("EEPROM    : 1 KB");
}

void cmd_clear()
{
    for(int i = 0; i < 40; i++)
    {
        Serial.println();
    }
}

void cmd_ls()
{
    fs_list();
}

void cmd_mkdir(char name[])
{
    if(fs_createDirectory(name))
        Serial.println("Directory created.");
    else
        Serial.println("Cannot create directory.");
}

void cmd_touch(char name[])
{
    if(fs_createFile(name))
        Serial.println("File created.");
    else
        Serial.println("Cannot create file.");
}

void cmd_cd(char name[])
{
    if(fs_changeDirectory(name))
        return;

    Serial.println("Directory not found.");
}

void cmd_pwd()
{
    fs_printWorkingDirectory();
}

void cmd_write(char filename[])
{
    enterWriteMode(filename);
}

void cmd_cat(char filename[])
{
    if(!fs_readFile(filename))
        Serial.println("File not found.");
}

void cmd_format()
{
    fs_format();
}

void cmd_tetris()
{
    const int BOARD_WIDTH = 10;
    const int BOARD_HEIGHT = 20;
    const int dropInterval = 600;

    // Tetromino Shape Definitions (4x4 Matrix)
    const uint8_t SHAPES[7][4][4] = {
      // Index 0: Cyan -> I-Piece
      {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
      // Index 1: Blue -> J-Piece
      {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
      // Index 2: Yellow -> L-Piece
      {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
      // Index 3: White -> O-Piece
      {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
      // Index 4: Green -> S-Piece
      {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
      // Index 5: Magenta -> T-Piece
      {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
      // Index 6: Red -> Z-Piece
      {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}
    };

    // ANSI Color Code Palette
    const char* ANSI_COLORS[8] = {
      "\033[0m",        // 0: Reset
      "\033[1;36m",     // 1: Cyan (I)
      "\033[1;34m",     // 2: Blue (J)
      "\033[1;33m",     // 3: Yellow (L)
      "\033[1;37m",     // 4: White (O)
      "\033[1;32m",     // 5: Green (S)
      "\033[1;35m",     // 6: Magenta (T)
      "\033[1;31m"      // 7: Red (Z)
    };

    uint8_t board[20][10] = {0};
    uint8_t currentPiece[4][4];
    int currentType = 0;
    int currentX = 3;
    int currentY = 0;
    unsigned long lastDropTime = millis();
    long score = 0;
    bool gameOver = false;
    bool playing = true;

    randomSeed(analogRead(0));

    auto isValidPosition = [&](int newX, int newY, const uint8_t piece[4][4]) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (piece[r][c]) {
                    int targetX = newX + c;
                    int targetY = newY + r;

                    if (targetX < 0 || targetX >= BOARD_WIDTH || targetY >= BOARD_HEIGHT) return false;
                    if (targetY >= 0 && board[targetY][targetX] != 0) return false;
                }
            }
        }
        return true;
    };

    auto spawnPiece = [&]() {
        currentType = random(0, 7);
        currentX = 3;
        currentY = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                currentPiece[r][c] = SHAPES[currentType][r][c];
            }
        }
    };

    auto renderScreen = [&]() {
        Serial.print("\033[H"); 
        Serial.print("--- TETRIS (Score: "); 
        Serial.print(score); 
        Serial.print(" ) ---\r\n+--------------------+\r\n");

        for (int r = 0; r < BOARD_HEIGHT; r++) {
            Serial.print("|");
            for (int c = 0; c < BOARD_WIDTH; c++) {
                int colorIdx = board[r][c];

                if (r >= currentY && r < currentY + 4 && c >= currentX && c < currentX + 4) {
                    if (currentPiece[r - currentY][c - currentX]) {
                        colorIdx = currentType + 1;
                    }
                }

                if (colorIdx > 0) {
                    Serial.print(ANSI_COLORS[colorIdx]);
                    Serial.print("[]");
                    Serial.print(ANSI_COLORS[0]);
                } else {
                    Serial.print("  ");
                }
            }
            Serial.print("|\r\n");
        }

        Serial.print("+--------------------+\r\n");
        Serial.print("A/D (Left/Right), W (Rotate), S (Drop), Q (Quit)\r\n");
    };

    auto movePiece = [&](int newX, int newY) {
        if (isValidPosition(newX, newY, currentPiece)) {
            currentX = newX;
            currentY = newY;
            renderScreen();
            return true;
        }
        return false;
    };

    auto rotatePiece = [&]() {
        uint8_t temp[4][4];
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                temp[c][3 - r] = currentPiece[r][c];
            }
        }
        if (isValidPosition(currentX, currentY, temp)) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    currentPiece[r][c] = temp[r][c];
                }
            }
            renderScreen();
        }
    };

    auto lockPiece = [&]() {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (currentPiece[r][c]) {
                    int boardY = currentY + r;
                    int boardX = currentX + c;
                    if (boardY >= 0 && boardY < BOARD_HEIGHT) {
                        board[boardY][boardX] = currentType + 1;
                    }
                }
            }
        }
    };

    auto clearLines = [&]() {
        int linesCleared = 0;
        for (int r = BOARD_HEIGHT - 1; r >= 0; r--) {
            bool full = true;
            for (int c = 0; c < BOARD_WIDTH; c++) {
                if (board[r][c] == 0) { full = false; break; }
            }

            if (full) {
                linesCleared++;
                for (int moveR = r; moveR > 0; moveR--) {
                    for (int c = 0; c < BOARD_WIDTH; c++) {
                        board[moveR][c] = board[moveR - 1][c];
                    }
                }
                for (int c = 0; c < BOARD_WIDTH; c++) board[0][c] = 0;
                r++;
            }
        }

        switch (linesCleared) {
            case 1: score += 100; break;
            case 2: score += 300; break;
            case 3: score += 500; break;
            case 4: score += 800; break;
        }
    };

    Serial.print("\033[?25l");
    Serial.print("\033[2J");

    spawnPiece();
    renderScreen();

    while (playing && !gameOver) {
        if (Serial.available() > 0) {
            char input = Serial.read();
            switch (input) {
                case 'a': case 'A': movePiece(currentX - 1, currentY); break;
                case 'd': case 'D': movePiece(currentX + 1, currentY); break;
                case 's': case 'S': movePiece(currentX, currentY + 1); break;
                case 'w': case 'W': rotatePiece(); break;
                case 'q': case 'Q': playing = false; break;
            }
        }

        if (millis() - lastDropTime > dropInterval) {
            if (!movePiece(currentX, currentY + 1)) {
                lockPiece();
                clearLines();
                spawnPiece();
                if (!isValidPosition(currentX, currentY, currentPiece)) {
                    gameOver = true;
                    Serial.print("\033[2J\033[H\r\n   *** GAME OVER ***   \r\n   Final Score: ");
                    Serial.print(score);
                    Serial.print("\r\n");
                    break;
                }
            }
            lastDropTime = millis();
            renderScreen();
        }
    }

    Serial.print("\033[?25h");
    Serial.println("\nExited Tetris.");
}
