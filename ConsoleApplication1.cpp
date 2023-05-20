#include <iostream>
#include <string>
#include <vector>

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

Color GetColor(int color) {

	if (color == black) {
		return black;
	}
	else if (color == white) {
		return white;
	}
	else {
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
	char GetName() const {
		return name;
	}
};

class ChessFigure : public Figure {};

class Queen : public ChessFigure {

};


class Cell {
private:
	Point coord = Point();

	bool chekHit = false;
	Color color;
	Figure figure = Figure();

public:
	int hit = 0;

	Cell(Point point, Color color_ = noColor) : coord(point), color(color_) {}
	~Cell() {}



	bool operator == (char name) const {
		return figure.GetName() == name;
	}

	void Set(Figure figure_) {
		figure = figure_;
	}

	void Clear() {
		figure = Figure();
	}


	std::string CreateCellName(int column, int _string) {
		std::string symbol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string name = symbol[column] + std::to_string(_string + 1);
		return name;
	}

	const Point GetPoint() const {
		return coord;
	}


	friend std::ostream& operator << (std::ostream& out, Cell cell) {
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


		bool operator < (Iterator it) const {
			return current < it.current;
		}

		bool operator != (Cell* const c) const {
			return current != c;
		}



		//опреатор возврастания на один
		Iterator operator ++() {
			return Iterator(current++);
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

		Point GetPoint() const {
			return current->GetPoint();
		}

		//функция получения позиции
		Iterator get(Point point) const {
			return Iterator(current + size * point.GetColumn() + point.GetString());
		}

		//оператор прохода по горизонтали вперёд
		Iterator operator +(int string) const {
			return Iterator(current + string);
		}

		//оператор прохода по вертикали вверх
		Iterator pc(int column) const {
			return Iterator(current + size * column);
		}

		//оператор прохода по горизонтали назад
		Iterator operator -(int string) const {
			return Iterator(current - string);
		}

		//оператор прохода по вертикали вниз
		Iterator operator -- (int column) const {
			return Iterator(current - size * column);
		}

		//оператор прохода по диагонали до верхнего левого угла доски 
		Iterator vl(int index) const {
			return Iterator(current + size * index - index);
		}

		//оператор прохода по диагонали до верхнего правого угла доски
		Iterator vp(int index) const {
			return Iterator(current + size * index + index);
		}

		//оператор прохода по диагонали до нижнего правого угла доски
		Iterator np(int index) const {
			return Iterator(current - size * index + index);
		}

		//оператор прохода по диагонали до левого нижнего угла доски
		Iterator nl(int index) const {
			return Iterator(current - size * index - index);
		}

		bool IsFigure(char type) {
			return *current == type;
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
	const Cell& operator [](Point newCoord) const {
		return board[newCoord.GetString()][newCoord.GetColumn()];
	}

	//оператор получения координаты (неконстантный метод)
	Cell& operator [](Point newCoord) {
		return board[newCoord.GetString()][newCoord.GetColumn()];
	}

	int GetSize() const {
		return size;
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
	friend std::ostream& operator << (std::ostream& out,Board board) {
		std::cout << board.CreatCoordColumn() << std::endl;
		for (int i = board.GetSize(); i >= 0; i--) {
			std::cout << i + 1;
			for (int j = 0; j < board.GetSize(); j++) {
				std::cout << board[i][j];
			}
			std::cout << " " << i + 1;
			std::cout << "\n";
		}
		std::cout << board.CreatCoordColumn() << std::endl;
	}
};

class Player {
private:
	Color side;

public:
	Player(Color side_) :side(side_) {}
	~Player() {}
	virtual void Move(Figure figure, Point newCoord, Point oldCoord) {}
	virtual void Set(Figure figure, Point newCoord) {}
	virtual void Remove(Point oldCoord) {}
};

class Players {
private:
	int size;
	Player* players;
public:
	Players(int size_) :players(nullptr), size(size_) {
		for (int i = 1; i <= size; i++) {
			players[i] = Player(GetColor(i));
		}
	}
	~Players() {
		delete[] players;
	}

	Player operator [] (int index) const {
		return players[index];
	}

};



//Класс GameRules
class GameRules {
public:

	Board board;

	virtual void Move(Figure figure, Point newCoord, Point oldCoord) {}
	virtual void Set(Figure figure, Point newCoord) {
		if (board[newCoord] == 'Z') {
			board[newCoord].Set(figure);
		}
	}
	void Remove(Point oldCoord) {
		if (!(board[oldCoord] == 'Z')) {
			board[oldCoord].Clear();
		}
	}

};

//Класс Queens c наследованием класса GameRules для расстановки максимального количества небьющих друг друга ферзей
class Queens : public GameRules {
public:


	//Функция, вызывающая реурсивную функцию. Возвращает доску с расставленными королевами
	Board CallPartingQueen(int numPlacedQueens) {
		std::vector<Cell>* QueensInCell = new std::vector<Cell>[board.GetSize()];
		board = PartingQueen(numPlacedQueens, 0, 0, 0, QueensInCell);
		return board;
	}

private:
	const int numPlayers = 1;

	int GetNumPlayers() {
		return numPlayers;
	}

	//Функция для проверки клетки на наличие королевы на ней
	bool CheckHit(Cell cell) {
		return !(cell == 'Z');
	}

	//функция для проверки клетки на отсутствие королевы на ней
	bool CheckEmpty(Cell cell) {
		return cell == 'Z';
	}

	//Функция для перемещение королевы на другую клетку
	//переделать в SET
	void Move(Point PointQueen) {
		bool isHit = !CheckHit(board[PointQueen]);
		HorizonMoving(PointQueen, isHit);
		VerticalMoving(PointQueen, isHit);
		DiagonalMoving(PointQueen, isHit);
	}

	//Функция для проверки количества королев на доске
	int CheckBoard() {
		int numQueen = 0;
		for (Board::Iterator it = it.begin(); it != it.end(); ++it) {
			if (it.IsFigure('Q')) {
				numQueen += 1;
			}
		}
		return numQueen;
	}

	//Функция для проверки находится ли королева под боем другой королевы
	void FigureHit(Point point, bool isHit) {
		if (isHit) {
			board[point].hit += 1;
		}
		else {
			board[point].hit = 1;
		}
	}

	//Функция для перемещения по горизонтали
	void HorizonMoving(Point point, bool isHit) {
		Point point_ = point;
		for (Board::Iterator it = it.get(point); it < it.get(point) + board.GetSize(); ++it) {
			if (&*it != &board[point]) {
				FigureHit(it.GetPoint(), isHit);
			}
		}
	}

	//Функция для перемещения по вертикали
	void VerticalMoving(Point point, bool isHit) {
		Point point_ = point;
		for (Board::Iterator it = it.get(point); it < it.get(point).pc(board.GetSize()); ++it) {
			if (&*it != &board[point]) {
				FigureHit(it.GetPoint(), isHit);
			}
		}
	}

	//Функция для перемещения по горизонтали
	void DiagonalMoving(Point point, bool isHit) {
		Point point_ = point;
		Board::Iterator it = it.get(point);
		for (int i = 0; i < board.GetSize(); i++) {
			FigureHit(it.vp(i).GetPoint(), isHit);
			FigureHit(it.vl(i).GetPoint(), isHit);
			FigureHit(it.np(i).GetPoint(), isHit);
			FigureHit(it.nl(i).GetPoint(), isHit);
		}
	}

	//Функция, которая решает какую королеву надо изменить и возвращает индекс этой королевы
	int ReturnPreviousQueen(Cell* QueensInCell[], int numPlacedQueens) {
		Point point;
		do {
			point = QueensInCell[numPlacedQueens]->GetPoint();
			numPlacedQueens -= 1;
		} while (point.GetString() > board.GetSize());
		return numPlacedQueens;
	}

	//Функция для клетки доски, которая ставит королеву на неё и бьёт все клетки по горизонтали, вертикали и диагонали

	//Рекурсивная функция, которая ставит королеву и вызывает функцию для постановки следующей королевы
	Board PartingQueen(int numPlacedQueens, int numQueen, int column, int string, std::vector<Cell>* QueensInCell) {
		if (CheckBoard() == board.GetSize()) {
			return board;
		}
		for (int i = 0; i < 8; i++) {
			if (CheckEmpty(board[string][i]) && CheckHit(board[string][i]) == false) {
				Move(Point(string, i));
				Cell cell(Point(string, i));

				QueensInCell->push_back(cell);
				if (string < 8) {
					board = PartingQueen(numPlacedQueens, numQueen, 0, string + 1, QueensInCell);
					if (CheckBoard() == board.GetSize()) {
						return board;
					}
					Remove(Point(string, i));
					QueensInCell->pop_back();
				}
			}
		}
		return board;
	}


};


//Шаблонный класс для игровой сессии
template <typename GameType>
class Match {
	GameType gr;
	Players players(gr.GetNumPlayers());
public:
	Match(GameType gr_) :gr(gr_) {}

	int Game() {
		return 1;
	}
};

int main() {
	int colQueens;
	std::cin >> colQueens;
	Board board(colQueens);

	GameRules:Queens queens();
	queens().CallPartingQueen(colQueens);

	Match<Queens> match(queens());

	return 1;
}
