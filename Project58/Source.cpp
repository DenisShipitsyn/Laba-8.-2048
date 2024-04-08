#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "glut.h"
const int SIZE = 4;
const int TILE_SIZE = 100;
int board[SIZE][SIZE] = { 0 };
int score = 0; // Переменная для хранения очков

void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SIZE * TILE_SIZE, 0, SIZE * TILE_SIZE);

    glLineWidth(2.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawTile(int x, int y, int value) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    switch (value) {
    case 2: glColor3f(0.9f, 0.9f, 0.9f); break;
    case 4: glColor3f(1.0f, 1.0f, 0.5f); break;
    case 8: glColor3f(1.0f, 0.5f, 0.0f); break;
    case 16: glColor3f(1.0f, 0.0f, 0.0f); break;
    case 32: glColor3f(0.8f, 0.6f, 1.0f); break;
    case 64: glColor3f(0.0f, 0.0f, 1.0f); break;
    case 128: glColor3f(0.0f, 1.0f, 1.0f); break;
    case 256: glColor3f(0.6f, 1.0f, 0.6f); break;
    case 512: glColor3f(0.0f, 1.0f, 0.0f); break;
    case 1024: glColor3f(0.0f, 0.5f, 0.0f); break;
    case 2048: glColor3f(1.0f, 0.0f, 1.0f); break;
    default: glColor3f(1.0f, 1.0f, 1.0f);
    }

    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(TILE_SIZE, 0);
    glVertex2i(TILE_SIZE, TILE_SIZE);
    glVertex2i(0, TILE_SIZE);
    glEnd();

    if (value != 0) {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string text = std::to_string(value);
        glRasterPos2i(TILE_SIZE / 2 - 8 * text.length(), TILE_SIZE / 2 + 8);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(TILE_SIZE, 0);
    glVertex2i(TILE_SIZE, TILE_SIZE);
    glVertex2i(0, TILE_SIZE);
    glEnd();

    glPopMatrix();
}

void generateTile() {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyCells.push_back(std::make_pair(i, j));
            }
        }
    }
    if (emptyCells.empty()) {
        return;
    }
    int position = rand() % emptyCells.size();
    int x = emptyCells[position].first;
    int y = emptyCells[position].second;
    board[x][y] = 2;
}

bool checkWin() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 2048) {
                return true;
            }
        }
    }
    return false;
}

bool checkLose() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                return false;
            }
            if (j > 0 && (board[i][j] == board[i][j - 1] || board[i][j - 1] == 0)) {
                return false;
            }
            if (j < SIZE - 1 && (board[i][j] == board[i][j + 1] || board[i][j + 1] == 0)) {
                return false;
            }
            if (i > 0 && (board[i][j] == board[i - 1][j] || board[i - 1][j] == 0)) {
                return false;
            }
            if (i < SIZE - 1 && (board[i][j] == board[i + 1][j] || board[i + 1][j] == 0)) {
                return false;
            }
        }
    }
    return true;
}

void moveLeft() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE - 1; ++j) {
            if (board[i][j] == 0) {
                for (int k = j + 1; k < SIZE; ++k) {
                    if (board[i][k] != 0) {
                        board[i][j] = board[i][k];
                        board[i][k] = 0;
                        break;
                    }
                }
            }
        }
    }
}

void mergeLeft() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE - 1; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveRight() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = SIZE - 1; j > 0; --j) {
            if (board[i][j] == 0) {
                for (int k = j - 1; k >= 0; --k) {
                    if (board[i][k] != 0) {
                        board[i][j] = board[i][k];
                        board[i][k] = 0;
                        break;
                    }
                }
            }
        }
    }
}

void mergeRight() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = SIZE - 1; j > 0; --j) {
            if (board[i][j] == board[i][j - 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j - 1] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveUp() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE - 1; ++i) {
            if (board[i][j] == 0) {
                for (int k = i + 1; k < SIZE; ++k) {
                    if (board[k][j] != 0) {
                        board[i][j] = board[k][j];
                        board[k][j] = 0;
                        break;
                    }
                }
            }
        }
    }
}

void mergeUp() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE - 1; ++i) {
            if (board[i][j] == board[i + 1][j] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveDown() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = SIZE - 1; i > 0; --i) {
            if (board[i][j] == 0) {
                for (int k = i - 1; k >= 0; --k) {
                    if (board[k][j] != 0) {
                        board[i][j] = board[k][j];
                        board[k][j] = 0;
                        break;
                    }
                }
            }
        }
    }
}

void mergeDown() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = SIZE - 1; i > 0; --i) {
            if (board[i][j] == board[i - 1][j] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i - 1][j] = 0;
                score += board[i][j];
            }
        }
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
}

void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        moveLeft();
        mergeLeft();
        moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        moveRight();
        mergeRight();
        moveRight();
        break;
    case GLUT_KEY_UP:
        moveUp();
        mergeUp();
        moveUp();
        break;
    case GLUT_KEY_DOWN:
        moveDown();
        mergeDown();
        moveDown();
        break;
    }
    generateTile();
    glutPostRedisplay();
    if (checkWin()) {
        std::cout << "You win! Your score: " << score << std::endl;
        exit(0);
    }
    if (checkLose()) {
        std::cout << "Game over! Your score: " << score << std::endl;
        exit(0);
    }
}

void drawBoard() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int x = j * TILE_SIZE;
            int y = (SIZE - 1 - i) * TILE_SIZE;
            drawTile(x, y, board[i][j]);
        }
    }
}

void timer(int value) {
    glutTimerFunc(1000, timer, 0);
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoard();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(time(nullptr));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SIZE * TILE_SIZE, SIZE * TILE_SIZE);
    glutCreateWindow("2048 Game");
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    init();
    generateTile();
    generateTile();
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();
    return 0;
}
