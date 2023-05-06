#include <iostream>
#include <string>

enum Color {
	black = 1,
	white = 2,
	noColor = 0,
};

Color RelplaceColor(Color color) {
	switch (color) {
	case black:
		return white;
	case white:
		return black;
	case noColor:
		return noColor;
	default:
		return noColor;
	}
}

class Point {
private:
	int string;
	int column;
public:
	Point(int string_ = -1, int column_ = -1) :string(string_), column(column_) {}
	~Point() {}
	void SetString(int string_) {
		string = string_;
	}
	void SetColumn(int column_) {
		column = column_;
	}
	int GetString() {
		return string;
	}
	int GetColumn() {
		return column;
	}
};

class Figure {
private:
	Color color;
	char name;
public:
	Figure(char name_ = 'Z', Color color_ = noColor) : color(color_), name(name_) {}
	~Figure() {}
	char GetName() {
		return name;
	}
};

class ChessFigure : public Figure {};

class Queen : public ChessFigure {

};

class Cell {
private:
	Point coord = Point();
	int hit = 0;
	bool chekHit = false;
	Color color;
	Figure figure = Figure();

public:
	Cell(Point point, Color color_ = noColor) : coord(point), color(color_) {}
	~Cell() {}

	std::string CreateCellName(int column, int _string) {
		std::string symbol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string name = symbol[column] + std::to_string(_string + 1);
		return name;
	}

	void SetCell(Cell& cell, int string, int column) {
		cell.coord.SetString(string);
		cell.coord.SetColumn(column);
	}

	friend std::ostream& operator << (std::ostream& out, Cell& cell) {
		std::cout << " ";
		if (cell.figure.GetName() == 'Z') {
			if (cell.color == 1) {
				std::cout << "-";
			}
			else {
				std::cout << "*";
			}
		}
		else {
			std::cout << cell.figure.GetName();
		}
	}
};

class Board {
private:
	int size;
	Cell** board;
public:
	class Iterator {
		Cell* current;
		int size;
	public:
		Iterator(Cell* board) :current(board) {}

		//опреатор указателя
		const Cell operator *() {
			return *current;
		}
		
		//опреатор указателя
		const Cell operator ->() {
			return *current;
		}

		//опреатор равенства
		bool operator == (Iterator it) const {
			return current == it.current;
		}

		//опреатор неравенства
		bool operator != (Iterator it) const {
			return current != it.current;
		}

		//опреатор возврастания на один
		Iterator operator ++() {
			return current++;
		}
		
		//опреатор уменьшения на один
		Iterator operator --() {
			return current--;
		}

		//функция получения начальной позиции
		Iterator begin() const {
			return Iterator(current);
		}

		//функция получения конечной позиции
		Iterator end() const {
			return Iterator(current + size);
		}

		//функция получения позиции
		Iterator get(Point point) const {
			return Iterator(current[point.GetString][point.GetColumn]);
		}

		//оператор прохода по горизонтали вперёд
		Iterator operator +(int string) const {
			return Iterator(current[point.GetString + string][point.GetColumn]);
		}

		//оператор прохода по вертикали вверх
		Iterator operator |+ (int column) const {
			return Iterator(current[point.GetString][point.GetColumn + column]);
		}

		//оператор прохода по горизонтали назад
		Iterator operator -(int string) const {
			return Iterator(current[point.GetString - string][point.GetColumn]);
		}

		//оператор прохода по вертикали вниз
		Iterator operator |-(int column) const {
			return Iterator(current[point.GetString][point.GetColumn - column]);
		}

		//оператор прохода по диагонали до верхнего левого угла доски 
		Iterator operator \+ (int index) const {
			return Iterator(current[point.GetString - index][point.GetColumn + index]);
		}

		//оператор прохода по диагонали до верхнего правого угла доски
		Iterator operator /+ (int index) const {
			return Iterator(current[point.GetString + index][point.GetColumn + index]);
		}

		//оператор прохода по диагонали до нижнего правого угла доски
		Iterator operator \- (int index) const {
			return Iterator(current[point.GetString + index][point.GetColumn - index]);
		}

		//оператор прохода по диагонали до левого нижнего угла доски
		Iterator operator /- (int index) const {
			return Iterator(current[point.GetString - index][point.GetColumn - index]);
		}
	};

	//конструктор для доски
	Board(int size_) : size(size_) {
		CreateBoard();
	}

	//деструктор для доски
	~Board() {
		delete[] board;
	}

	//оператор получения координаты (константный метод)
	const Cell& operator [](Point newCoord)const {
		return board[newCoord.GetString()][newCoord.GetColumn()];
	}

	//оператор получения координаты (неконстантный метод)
	Cell& operator [](Point newCoord) {
		return board[newCoord.GetString()][newCoord.GetColumn()];
	}

	//Прокси класс для получения второй координаты
	class Proxy {
	private:
		Cell* board;
	public:
		Proxy(Cell* _array) : board(_array) { }
		Cell operator[](int j) {
			return board[j];
		}
	};

	//Оператор квадратные скобочки для получения второй координаты по горизонтали
	Proxy operator[](int i) {
		return Proxy(board[i]);
	}

	//Функция для создания шахматной доски
	void CreateBoard() {
		Color color = white;
		for (int i = 0; i < size; i++) {
			color = RelplaceColor(color);
			for (int j = 0; j < size; j++) {
				color = RelplaceColor(color);
				Point point(i, j);
				board[i][j] = Cell(point, color);
			}
		}
	}

	//Функция для создания координат буквенных обозначений столбцов
	std::string CreatCoordColumn() {
		std::string res;
		std::string coordColumn = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		if (size <= 27) {
			for (int i = 1; i < size; i++) {
				if (i == size - 1) {
					res = res + '|' + coordColumn[i] + '|';
				}
				else {
					res = res + '|' + coordColumn[i];
				}
			}
		}
		else if (size <= 27 * 27) {
			for (int i = 0; i <= (int)size / 27; i++) {
				for (int j = 1; (j < size - i * 27 and j < 27); j++) {
					if (j + i * 27 == size - 1) {
						res = res + '|' + coordColumn[i] + coordColumn[j] + '|';
					}
					else {
						res = res + '|' + coordColumn[i] + coordColumn[j];
					}
				}
			}
		}
		return res;
	}

	//Оператор для вывода доски
	friend std::ostream& operator << (std::ostream& out, Board* board) {
		std::cout << board->CreatCoordColumn() << std::endl;
		for (int i = board->size; i >= 0; i--) {
			std::cout << i + 1;
			for (int j = 0; j < board->size; j++) {
				std::cout << board->board[i][j]
			}
			std::cout << " " << i + 1;
			std::cout << "\n";
		}
		std::cout << board->CreatCoordColumn() << std::endl;
	}
}

//Шаблонный класс для игровой сессии
template <class GameType>
class Match {
	GameType gr;
	Players* players;
public:
	Match(GameType gr_) :gr(gr_) {}
};

//Класс GameRules
class GameRules {
private:
	Board board;
public:
	virtual void Move(Figure figure, Point newCoord, Point oldCoord) = 0;
	virtual void Set(Figure figure, Point newCoord) {
		if (!board.check(newCoord)) {
			board[newCoord] = figure;
		}
	}
	void Remove(Figure figure, Point oldCoord) {
		if (board.Check(newCoord) {
			board[oldCoord] = std::nullptr;
		}
	}
};

//Класс Queens c наследованием класса GameRules для расстановки максимального количества небьющих друг друга ферзей
class Queens :public GameRules {
	CheckHit ch;
private:
	//Функция для расстановки фигуры на доску
	void Set(Figure figure, Point newCoord) {
		if (!CheckHit(newCoord)) {
			board[newCoord] = figure;
		}
	}

	//Функция для проверки клетки на наличие королевы на ней
	bool CheckHit(Cell* cell) {
		return !(cell->figure.name == 'Z');
	}

	//функция для проверки клетки на отсутствие королевы на ней
	bool CheckEmpty(Cell* cell) {
		return cell->figure.name == 'Z';
	}

	//Функция для перемещение королевы на другую клетку
	void Move(Point PointQueen) {
		bool isHit = !board.Check(newCoord);
		HorizonMoving(PointQueen, isHit);
		VerticalMoving(PointQueen, isHit);
		DiagonalMoving(PointQueen, isHit);
	}

	//Функция для проверки количества королев на доске
	int CheckBoard(Board* board) {
		int numQueen = 0;
		for (Iterator it=board.begin(); it !=  board.end(); it++) {
			if (it->figure.type == 'Q') {
				numQueen += 1;
			}
		}
		return numQueen;
	}

	//Функция для проверки находится ли королева под боем другой королевы
	void FigureHit(Point point, bool isHit) {
		if (isHit) {
			ch[point] += 1;
		}
		else {
			ch[point] -= 1;
		}
	}

	//Функция для перемещения по горизонтали
	void HorizonMoving(Point point, bool isHit) {
		Point point_ = point;
		for (Iterator it = board.get(point); it < board.get(point) + board->size; it++) {
			if (it != board[point]) {
				FigureHit(it->coord);
			}
		}
	}

	//Функция для перемещения по вертикали
	void VerticalMoving(Point point, bool isHit) {
		Point point_ = point;
		for (Iterator it = board.get(point); it < board.get(point) |+ board->size; it++) { 
			if (it!= board[point]) {
				FigureHit(it->coord);
			}
		}
	}

	//Функция для перемещения по горизонтали
	void DiagonalMoving(Point point, bool isHit) {
		Point point_ = point;
		Iterator it = board.get(point);
		for (int i = 0; i < board->size; i++) {
			FigureHit((it /+ i)->coord);
			FigureHit((it \+ i)->coord);
			FigureHit((it \- i)->coord);
			FigureHit((it /- i)->coord);
		}
	}

	//Функция, которая решает какую королеву надо изменить и возвращает индекс этой королевы
	int ReturnPreviousQueen(ChessBoard* board, Cell* QueensInCell[8], int numPlacedQueens) {
		Point point;
		do {
			point = QueensInCell[numPlacedQueens]->coord;
			numPlacedQueens -= 1;
		} while (point.x > 8);
		return numPlacedQueens;
	}

	//Функция для клетки доски, которая ставит королеву на неё и бьёт все клетки по горизонтали, вертикали и диагонали

	//Рекурсивная функция, которая ставит королеву и вызывает функцию для постановки следующей королевы
	ChessBoard* PartingQueen(ChessBoard* board, int numPlacedQueens, int numQueen, int column, int string, Cell* QueensInCell[8]) {
		if (CheckBoard(board) == 8) {
			return board;
		}
		for (int i = 0; i < 8; i++) {
			if (CheckEmpty(board->board[string][i]) && board->board[string][i]->checkHit == false) {
				MoveQueen(board, board->board[string][i], 'Q');
				Cell* cell = new Cell;
				GetCell(cell, string, i);
				QueensInCell[numQueen] = cell;
				if (string < 8) {
					board = PartingQueen(board, numPlacedQueens, numQueen, 0, string + 1, QueensInCell);
					if (CheckBoard(board) == 8) {
						return board;
					}
					MoveQueen(board, board->board[string][i], 'Z');
				}
			}
		}
		return board;
	}

	//Функция, вызывающая реурсивную функцию. Возвращает доску с расставленными королевами
	ChessBoard* CallPartingQueen(ChessBoard* board, int numPlacedQueens) {
		Cell* QueensInCell[8];
		board = PartingQueen(board, numPlacedQueens, 0, 0, 0, QueensInCell);
		return board;
	}
};
