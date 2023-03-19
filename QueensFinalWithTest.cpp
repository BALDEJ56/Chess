
#include <iostream>
#include <string>

enum Color {
	black = "*",
	white = "-"
};

//Структура "Фигура" хранит данные о цвете, тип фигуры и отношение к другим фигурам
struct Figure {
	char type = 'Z';
	Color color;
	bool allAgresive;
};

//Структура координат фигуры
struct Point {
	int x;
	int y;
};

//Структура "Клетка" хранит данные о координатах клетки, находиться ли клетка
//под боем и сколько раз её бьют, структуру данных "Фигура" и цвет клетки
struct Cell {
	Point coord;
	bool checkHit = false;
	int hit = 0;
	Figure figure;
	Color color;
};

//Стурктура "Шахматная доска" хранит массив ячеек
struct ChessBoard {
	Cell* board[8][8];

};

//Функция для создания названий каждой клетки
string CreateCellName(int column, int _string) {
	std::string symbol = "ABCDEFGH";
	std::string name = symbol[column] + to_string(_string + 1);
	return name;
}

//Функция для вывода состояния клетки
void PrintCell(Cell* cell) {
	std::cout << " ";
	if (cell->figure.type == 'Z') {
		if (!cell->checkHit) {
			/*
			if (cell->color) {
				std::cout << "-";
			}
			else {
				std::cout << "*";
			}
			*/
			std::cout << cell->color;
			
		}
		else {
			std::cout << "H";
		}
	}
	else {
		std::cout << cell->figure.type;
	}
}

//Функция для вывода шахматной доски
void PrintChessBoard(ChessBoard* board) {
	std::cout << "  A B C D E F G H" << std::endl;
	for (int i = 7; i >= 0; i--) {
		std::cout << i + 1;
		for (int j = 0; j < 8; j++) {
			PrintCell(board->board[i][j]);
		}
		std::cout << " " << i + 1;
		std::cout << "\n";
	}
	std::cout << "  A B C D E F G H" << std::endl;
}

//Функция для создания клетки с данными о координатах и цвете
Cell* CreateCell(int column, int string, Color color) {
	Cell* cell = new Cell;
	Point point;
	point.x = string;
	point.y = column;
	cell->coord = point;
	cell->color = color;
	return cell;
}

//Функция для замены цвета клетки
bool RelplaceColor(Color color) {
	if (color == white) {
		return black;
	}
	return white;
	
}

//Функция для создания шахматной доски
ChessBoard* CreateBoard(ChessBoard* board) {
	Color color = white;
	for (int i = 0; i < 8; i++) {
		color = RelplaceColor(color);
		for (int j = 0; j < 8; j++) {
			color = RelplaceColor(color);
			board->board[i][j] = CreateCell(i, j, color);
		}
	}
	return board;
}

//Функция для счёта ударов по клетке 
void FigureHit(Cell* cell, bool isHit) {
	if (isHit) {
		cell->hit += 1;
		cell->checkHit = true;
	}
	else {
		cell->hit -= 1;
		if (cell->hit == 0) {
			cell->checkHit = false;
		}
	}
}

//Функция для королевы, которая бьёт все клетки по горизонтали
ChessBoard* HorizonMovingQueen(ChessBoard* board, Cell* cell, bool isHit) {
	for (int i = 0; i < 8; i++) {
		if (board->board[cell->coord.y][i] != board->board[cell->coord.y][cell->coord.x]) {
			FigureHit(board->board[cell->coord.y][i], isHit);
		}
	}
	return board;
}

//Функция для королевы, которая бьёт все клетки по вертикали
ChessBoard* VerticalMovingQueen(ChessBoard* board, Cell* cell, bool isHit) {
	for (int i = 0; i < 8; i++) {
		if (board->board[i][cell->coord.x] != board->board[cell->coord.y][cell->coord.x]) {
			FigureHit(board->board[i][cell->coord.x], isHit);
		}
	}
	return board;
}

//Функция для королевы, которая бьёт все клетки по диагонали
ChessBoard* DiagonalMovingQueen(ChessBoard* board, Cell* cell, bool isHit) {
	for (int i = 0; i < 8; i++) {
		if (cell->coord.x + i < 8 && cell->coord.y + i < 8) {
			FigureHit(board->board[cell->coord.y + i][cell->coord.x + i], isHit);
		}
		if (cell->coord.x - i >= 0 && cell->coord.y - i >= 0) {
			FigureHit(board->board[cell->coord.y - i][cell->coord.x - i], isHit);
		}
		if (cell->coord.x - i >= 0 && cell->coord.y + i < 8) {
			FigureHit(board->board[cell->coord.y + i][cell->coord.x - i], isHit);
		}
		if (cell->coord.x + i < 8 && cell->coord.y - i >= 0) {
			FigureHit(board->board[cell->coord.y - i][cell->coord.x + i], isHit);
		}
	}
	return board;
}

//Функция, которая решает какую королеву надо изменить и возвращает индекс этой королевы
int ReturnPreviousQueen(ChessBoard* board, Cell* QueensInCell[8], int numPlacedQueens) {
	Point point;
	bool IsQueen = false;
	while (!IsQueen) {
		point.x = QueensInCell[numPlacedQueens]->coord.x;
		point.y = QueensInCell[numPlacedQueens]->coord.y;
		if (point.x > 8) {
			IsQueen = false;
		}
		else {
			IsQueen = true;
		}
		numPlacedQueens -= 1;
	}
	return numPlacedQueens;
}

//Функция для клетки доски, которая ставит королеву на неё и бьёт все клетки по горизонтали, вертикали и диагонали
ChessBoard* QueenMoving(ChessBoard* board, Cell* cellQueen) {
	cellQueen->figure.allAgresive = true;
	cellQueen->figure.type = 'Q';
	board = HorizonMovingQueen(board, cellQueen, true);
	board = VerticalMovingQueen(board, cellQueen, true);
	board = DiagonalMovingQueen(board, cellQueen, true);
	return board;
}

//Функция, убирающая нужную королеву и возвращающая доску без неё
ChessBoard* QueenReplace(ChessBoard* board, Cell* cellQueen) {
	std::cout << CreateCellName(cellQueen->coord.y, cellQueen->coord.x) << "\n";
	cellQueen->figure.allAgresive = false;
	cellQueen->figure.type = 'Z';
	std::cout << cellQueen->figure.type << "\n";
	board = HorizonMovingQueen(board, cellQueen, false);
	board = VerticalMovingQueen(board, cellQueen, false);
	board = DiagonalMovingQueen(board, cellQueen, false);
	return board;
}

//Функция для проверки количества королев на доске
int CheckBoard(ChessBoard* board) {
	int numQueen = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board->board[i][j]->figure.type == 'Q') {
				numQueen += 1;
			}
		}
	}
	std::cout << numQueen << "\n";
	return numQueen;
}

//Рекурсивная функция, которая ставит королеву и вызывает функцию для постановки следующей королевы
ChessBoard* RecursivePartingQueen(ChessBoard* board, int numPlacedQueens, int numQueen, int column, int string, Cell* QueensInCell[8]) {
	if (CheckBoard(board) == 8) {
		return board;
	}
	for (int i = 0; i < 8; i++) {
		if (board->board[string][i]->figure.type == 'Z' && board->board[string][i]->checkHit == false) {
			board = QueenMoving(board, board->board[string][i]);
			PrintChessBoard(board);
			Cell* cell = new Cell;
			cell->coord.x = string;
			cell->coord.y = i;
			QueensInCell[numQueen] = cell;
			if (string < 8) {
				board = RecursivePartingQueen(board, numPlacedQueens, numQueen, 0, string + 1, QueensInCell);
				if (CheckBoard(board) == 8) {
					return board;
				}
				board = QueenReplace(board, board->board[string][i]);
			}
		}
	}
	return board;
}

//Функция, вызывающая реурсивную функцию. Возвращает доску с расставленными королевами
ChessBoard* RecursiveCallPartingQueen(ChessBoard* board, int numPlacedQueens) {
	Cell* QueensInCell[8];
	board = RecursivePartingQueen(board, numPlacedQueens, 0, 0, 0, QueensInCell);
	return board;
}

//Функция, расставляющая при помощи циклов королевы
ChessBoard* PartingEightQueen(ChessBoard* board, int numPlacedQueens) {
	int numQueen = 0;
	Cell* QueensInCell[8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board->board[i][j]->figure.type == 'Z' && board->board[i][j]->checkHit == false) {
				numQueen++;
				board = QueenMoving(board, board->board[i][j]);
				if (numQueen == numPlacedQueens) {
					break;
				}
				PrintChessBoard(board);
				Cell* cell = new Cell;
				cell->coord.x = i;
				cell->coord.y = j;
				QueensInCell[numQueen] = cell;
			}
			if (numQueen == numPlacedQueens) {
				break;
			}
			if (numQueen < numPlacedQueens && i == 7 && j == 7) {
				int numberQueensNow = ReturnPreviousQueen(board, QueensInCell, numQueen);
				std::cout << "numQueen:" << numQueen << "numberQueensNow:" << numberQueensNow << '\n';
				for (int k = 0; k < numQueen - numberQueensNow; k++) {
					board = QueenReplace(board, board->board[QueensInCell[numQueen]->coord.x][QueensInCell[numQueen]->coord.y]);
				}
				numQueen -= numQueen - numberQueensNow;
				i = QueensInCell[numQueen + 1]->coord.x;
				j = QueensInCell[numQueen + 1]->coord.y;
			}
		}
		if (numQueen == numPlacedQueens) {
			break;
		}
	}
	return board;
}

//Функция, проверяющая вводимые значения
bool CheckHVD(string isTestMoving, char HVD) {
	for (int i = 0;i < isTestMoving.length();i++) {
		if (isTestMoving[i] == HVD) {
			return true;
		}
	}
	return false;
}

//Проверка по горизонтали
bool TestHorizontal(ChessBoard* board, Point pointQueen, Point pointQueenHorizontal) {
	board->board[pointQueen.x][pointQueen.y]->figure.allAgresive = true;
	board->board[pointQueen.x][pointQueen.y]->figure.type = 'Q';
	board = HorizonMovingQueen(board, board->board[pointQueen.x][pointQueen.y], true);
	PrintChessBoard(board);
	std::cout << CreateCellName(pointQueenHorizontal.y, pointQueenHorizontal.x) << '\n';
	if (board->board[pointQueenHorizontal.x][pointQueenHorizontal.y]->checkHit == false) {
		return true;
	}
	return false;
}

//Проверка по вертикали
bool TestVertical(ChessBoard* board, Point pointQueen, Point pointQueenVertical) {
	board->board[pointQueen.x][pointQueen.y]->figure.allAgresive = true;
	board->board[pointQueen.x][pointQueen.y]->figure.type = 'Q';
	board = VerticalMovingQueen(board, board->board[pointQueen.x][pointQueen.y], true);
	PrintChessBoard(board);
	std::cout << CreateCellName(pointQueenVertical.y, pointQueenVertical.x) << '\n';
	if (board->board[pointQueenVertical.x][pointQueenVertical.y]->checkHit == false) {
		return true;
	}
	return false;
}

//Проверка по диагонали
bool TestDiagonal(ChessBoard* board, Point pointQueen, Point pointQueenDiagonal) {
	board->board[pointQueen.x][pointQueen.y]->figure.allAgresive = true;
	board->board[pointQueen.x][pointQueen.y]->figure.type = 'Q';
	board = DiagonalMovingQueen(board, board->board[pointQueen.x][pointQueen.y], true);
	PrintChessBoard(board);
	std::cout << CreateCellName(pointQueenDiagonal.y, pointQueenDiagonal.x) << '\n';
	if (board->board[pointQueenDiagonal.x][pointQueenDiagonal.y]->checkHit == false){
		cout << true << endl;
		return true;
	}
	return false;
}

//Функция для задания координат двух проверяемых королев
bool TestQueenMoving(ChessBoard* board) {
	bool isTestTrue = true;
	Point pointQueen;
	std::cin >> pointQueen.x >> pointQueen.y;
	std::string isTestMoving;
	std::cout << "Start test? H or V or D: ";
	std::cin >> isTestMoving;
	if (CheckHVD(isTestMoving, 'H') or CheckHVD(isTestMoving, 'h')) {
		Point pointQueenHorizontal;
		std::cin >> pointQueenHorizontal.x >> pointQueenHorizontal.y;
		bool expectedResult;
		std::cin >> expectedResult;
		if (TestHorizontal(board, pointQueen, pointQueenHorizontal) != expectedResult) {
			return false;
		}
	}
	if (CheckHVD(isTestMoving, 'V') or CheckHVD(isTestMoving, 'v')) {
		Point pointQueenVertical;
		std::cin >> pointQueenVertical.x >> pointQueenVertical.y;
		bool expectedResult;
		std::cin >> expectedResult;
		std::cout << expectedResult;
		if (TestVertical(board, pointQueen, pointQueenVertical) != expectedResult) {
			return false;
		}		
	}
	if (CheckHVD(isTestMoving, 'D') or CheckHVD(isTestMoving, 'd')) {
		Point pointQueenDiagonal;
		std::cin >> pointQueenDiagonal.x >> pointQueenDiagonal.y;
		bool expectedResult;
		std::cin >> expectedResult;
		if (TestDiagonal(board, pointQueen, pointQueenDiagonal) != expectedResult) {
			return false;
		}	
	}
	return isTestTrue;
}

int main() {
	char isTest;
	ChessBoard* board = new ChessBoard;
	board = CreateBoard(board);
	std::cout << "Start test? Y or N: ";
	std::cin >> isTest;
	if (isTest == 'Y' || isTest == 'y') {
		bool test = TestQueenMoving(board);
		if (test) {
			std::cout << "Test passed successfully" << endl;
		}
		else {
			std::cout << "Test failed" << endl;
		}
	}
	else {
		int numPlacedQueens;
		std::cin >> numPlacedQueens;
		board = RecursiveCallPartingQueen(board, numPlacedQueens);
		PrintChessBoard(board);
		int a;
		std::cin >> a;
	}
	return 0;
}


