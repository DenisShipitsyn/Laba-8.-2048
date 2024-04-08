#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "glut.h"
const int SIZE = 4;
const int TILE_SIZE = 100;
int board[SIZE][SIZE] = { 0 };


void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SIZE * TILE_SIZE, 0, SIZE * TILE_SIZE);

    // Установка толщины линий
    glLineWidth(2.0);

    // Установка режима отображения полигонов
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Заполняем полигон цветом
}

void drawTile(int x, int y, int value) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Устанавливаем цвет плитки в зависимости от ее значения
    switch (value) {
    case 2:
        glColor3f(0.9f, 0.9f, 0.9f); // Белый для значения 2
        break;
    case 4:
        glColor3f(1.0f, 1.0f, 0.5f); // Бледно-желтый для значения 4
        break;
    case 8:
        glColor3f(1.0f, 0.5f, 0.0f); // Оранжевый для значения 8
        break;
    case 16:
        glColor3f(1.0f, 0.0f, 0.0f); // Красный для значения 16
        break;
    case 32:
        glColor3f(0.8f, 0.6f, 1.0f); // Светло-фиолетовый для значения 32
        break;
    case 64:
        glColor3f(0.0f, 0.0f, 1.0f); // Синий для значения 64
        break;
    case 128:
        glColor3f(0.0f, 1.0f, 1.0f); // Голубой для значения 128
        break;
    case 256:
        glColor3f(0.6f, 1.0f, 0.6f); // Светло-зеленый для значения 256
        break;
    case 512:
        glColor3f(0.0f, 1.0f, 0.0f); // Просто зеленый для значения 512
        break;
    case 1024:
        glColor3f(0.0f, 0.5f, 0.0f); // Темно-зеленый для значения 1024
        break;
    case 2048:
        glColor3f(1.0f, 0.0f, 1.0f); // Ярко-фиолетовый для значения 2048
        break;
    default:
        glColor3f(1.0f, 1.0f, 1.0f); // По умолчанию - белый
    }

    // Рисуем квадрат плитки
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(TILE_SIZE, 0);
    glVertex2i(TILE_SIZE, TILE_SIZE);
    glVertex2i(0, TILE_SIZE);
    glEnd();

    // Отрисовываем значение на плитке
    if (value != 0) {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string text = std::to_string(value);
        glRasterPos2i(TILE_SIZE / 2 - 8 * text.length(), TILE_SIZE / 2 + 8);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }
    glPopMatrix();
}
// Генерация новой двойки на случайной пустой клетке
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
        return; // Нет пустых клеток
    }
    int position = rand() % emptyCells.size(); // Случайная позиция для новой двойки
    int x = emptyCells[position].first;
    int y = emptyCells[position].second;
    board[x][y] = 2; // Генерируем новую двойку
}

// Проверка на победу (наличие двойки с числом 2048 на доске)
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

// Проверка на поражение (отсутствие пустых клеток и возможности сдвинуть фишки)
bool checkLose() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                return false; // Есть пустая клетка
            }
            // Проверяем возможность сдвинуть фишку влево
            if (j > 0 && (board[i][j] == board[i][j - 1] || board[i][j - 1] == 0)) {
                return false;
            }
            // Проверяем возможность сдвинуть фишку вправо
            if (j < SIZE - 1 && (board[i][j] == board[i][j + 1] || board[i][j + 1] == 0)) {
                return false;
            }
            // Проверяем возможность сдвинуть фишку вверх
            if (i > 0 && (board[i][j] == board[i - 1][j] || board[i - 1][j] == 0)) {
                return false;
            }
            // Проверяем возможность сдвинуть фишку вниз
            if (i < SIZE - 1 && (board[i][j] == board[i + 1][j] || board[i + 1][j] == 0)) {
                return false;
            }
        }
    }
    return true; // Нет пустых клеток и возможности сдвинуть фишки
}

// Сдвиг фишек влево
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

// Сложение фишек влево
void mergeLeft() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE - 1; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
            }
        }
    }
}

// Сдвиг фишек вправо
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

// Сложение фишек вправо
void mergeRight() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = SIZE - 1; j > 0; --j) {
            if (board[i][j] == board[i][j - 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j - 1] = 0;
            }
        }
    }
}

// Сдвиг фишек вверх
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

// Сложение фишек вверх
void mergeUp() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = 0; i < SIZE - 1; ++i) {
            if (board[i][j] == board[i + 1][j] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
            }
        }
    }
}

// Сдвиг фишек вниз
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

// Сложение фишек вниз
void mergeDown() {
    for (int j = 0; j < SIZE; ++j) {
        for (int i = SIZE - 1; i > 0; --i) {
            if (board[i][j] == board[i - 1][j] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i - 1][j] = 0;
            }
        }
    }
}


// Функция для обработки нажатий клавиш
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 27) { // Escape key
        exit(0);
    }
}

// Функция для обработки специальных клавиш
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
        std::cout << "You win!" << std::endl;
    }
    if (checkLose()) {
        std::cout << "Game over!" << std::endl;
    }
}

// Функция отрисовки доски
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

    glutTimerFunc(1000, timer, 0); // таймер с интервалом 1 секунда
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
    glutTimerFunc(1000, timer, 0); // таймер с интервалом 1 секунда
    glutMainLoop();
    return 0;
}

