#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "glut.h"
const int SIZE = 6;
const int TILE_SIZE = 100;
int board[SIZE][SIZE] = { 0 };
int originalBoard[SIZE][SIZE] = { 0 }; // Переменная для хранения копии доски
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

    if (x == 0 || y == 0 || x == (SIZE - 1) * TILE_SIZE || y == (SIZE - 1) * TILE_SIZE) {
        // Для внешних клеток используем темно-синий цвет
        glColor3f(0.1f, 0.1f, 0.5f);
    }
    else {
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
    for (int i = 1; i < SIZE - 1; ++i) {
        for (int j = 1; j < SIZE - 1; ++j) {
            if (board[i][j] == 0) {
                emptyCells.push_back(std::make_pair(i, j));
            }
        }
    }
    if (emptyCells.empty()) {
        return;
    }

    int value = (rand() % 10 == 0) ? 4 : 2;

    int position = rand() % emptyCells.size();
    int x = emptyCells[position].first;
    int y = emptyCells[position].second;
    board[x][y] = value;
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
    for (int i = 1; i < SIZE - 1; ++i) {
        for (int j = 1; j < SIZE - 1; ++j) {
            if (board[i][j] == 0) {
                for (int k = j + 1; k < SIZE - 1; ++k) {
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
    for (int i = 1; i < SIZE - 1; ++i) {
        for (int j = 1; j < SIZE - 1; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveRight() {
    for (int i = 1; i < SIZE - 1; ++i) {
        for (int j = SIZE - 2; j > 0; --j) {
            if (board[i][j] == 0) {
                for (int k = j - 1; k > 0; --k) {
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
    for (int i = 1; i < SIZE - 1; ++i) {
        for (int j = SIZE - 2; j > 0; --j) {
            if (board[i][j] == board[i][j - 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i][j - 1] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveUp() {
    for (int j = 1; j < SIZE - 1; ++j) {
        for (int i = 1; i < SIZE - 1; ++i) {
            if (board[i][j] == 0) {
                for (int k = i + 1; k < SIZE - 1; ++k) {
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
    for (int j = 1; j < SIZE - 1; ++j) {
        for (int i = 1; i < SIZE - 1; ++i) {
            if (board[i][j] == board[i + 1][j] && board[i][j] != 0) {
                board[i][j] *= 2;
                board[i + 1][j] = 0;
                score += board[i][j];
            }
        }
    }
}

void moveDown() {
    for (int j = 1; j < SIZE - 1; ++j) {
        for (int i = SIZE - 2; i > 0; --i) {
            if (board[i][j] == 0) {
                for (int k = i - 1; k > 0; --k) {
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
    for (int j = 1; j < SIZE - 1; ++j) {
        for (int i = SIZE - 2; i > 0; --i) {
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

void updateOriginalBoard() {
    std::memcpy(originalBoard, board, SIZE * SIZE * sizeof(int));
}

void handleSpecialKeypress(int key, int x, int y) {
    int originalBoard[SIZE][SIZE];
    std::memcpy(originalBoard, board, SIZE * SIZE * sizeof(int));

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

    bool moved = false;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (originalBoard[i][j] != board[i][j]) {
                moved = true;
                break;
            }
        }
        if (moved)
            break;
    }

    if (moved) {
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

void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        updateOriginalBoard(); // Сохраняем оригинальную доску перед перемещением клеток

        int row = SIZE - 1 - (y / TILE_SIZE);
        int col = x / TILE_SIZE;

        if ((row == 0 && col == 0) || // левый верхний угол
            (row == 0 && col == SIZE - 1) || // правый верхний угол
            (row == SIZE - 1 && col == 0) || // левый нижний угол
            (row == SIZE - 1 && col == SIZE - 1)) // правый нижний угол
        {
            // Если нажата угловая клетка внешнего слоя, ничего не делаем
            glutPostRedisplay();
            return;
        }

        if (row == 0 || row == SIZE - 1 || col == 0 || col == SIZE - 1) {
            // Нажатие на остальные клетки внешнего слоя
            if (row == 0) {
                // Нажатие на верхние клетки внешнего слоя, двигаем клетки вверх
                moveDown();
                mergeDown();
                moveDown();
            }
            else if (row == SIZE - 1) {
                // Нажатие на нижние клетки внешнего слоя, двигаем клетки вниз
                moveUp();
                mergeUp();
                moveUp();
            }
            else if (col == 0) {
                // Нажатие на левые клетки внешнего слоя, двигаем клетки влево
                moveLeft();
                mergeLeft();
                moveLeft();
            }
            else {
                // Нажатие на правые клетки внешнего слоя, двигаем клетки вправо
                moveRight();
                mergeRight();
                moveRight();
            }

            bool moved = false;
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    if (board[i][j] != originalBoard[i][j]) {
                        moved = true;
                        break;
                    }
                }
                if (moved)
                    break;
            }

            if (moved) {
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
            else {
                // Если клетки не изменили своего положения, новые клетки не появятся
                glutPostRedisplay();
            }
        }
    }
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
    glutMouseFunc(handleMouseClick); // Добавленная строка для обработки щелчков мыши
    init();
    generateTile();
    generateTile();
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();
    return 0;
}
