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

	bool operator == (Point point) {
		return string == point.GetString() and column == point.GetColumn();
	}

	bool operator != (Point point) {
		return !(string == point.GetString() and column == point.GetColumn());
	}

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

	bool operator != (Cell cell) {
		return coord != cell.GetPoint();
	}

	bool operator == (char name) const {
		return figure.GetName() == name;
	}

	void operator = (Cell cell) {
		coord = cell.GetPoint();
		color = cell.GetColor();
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

	const Color GetColor() const {
		return color;
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
		return out;
	}
};

class Board {
private:
	int size;
	std::vector<std::vector<Cell>> board;
public:
	class Iterator {
		Cell* current;
		int size;
	public:
		Iterator(Cell* board) :current(board) {}

		//опреатор указателя
		const Cell operator *() const {
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
	Board(int size_) : size(size_), board() {
		std::vector<std::vector<Cell>> board;
		CreateBoard();
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
		std::vector <Cell> board;
	public:
		Proxy(std::vector <Cell> _array) : board(_array) { }
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
			std::vector<Cell> lineBoard;
			for (int j = 0; j < size; j++) {
				color = RelplaceColor(color);
				Point point(i, j);
				lineBoard.push_back(Cell(point, color));
			}
			board.push_back(lineBoard);
		}
	}

	//Функция для создания координат буквенных обозначений столбцов
	std::string CreatCoordColumn() {
		std::string res;
		std::string coordColumn = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		if (size <= 27) {
			for (int i = 1; i < size + 1; i++) {
				if (i == size) {
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
	friend std::ostream& operator << (std::ostream& out, Board board) {
		std::cout << " ";
		std::cout << board.CreatCoordColumn() << std::endl;
		for (int i = board.GetSize() - 1; i >= 0; i--) {
			std::cout << i + 1;
			for (int j = 0; j < board.GetSize(); j++) {
				std::cout << board[Point(i, j)];
			}
			std::cout << " " << i + 1;
			std::cout << "\n";
		}
		std::cout << " ";
		std::cout << board.CreatCoordColumn() << std::endl;
		return out;
	}
};

class Player {
private:
	Color side;

public:
	Player(Color side_) :side(side_) {}
	~Player() {}

	void Set(Figure figure, Cell* newCell, Board board) {
		newCell->Set(figure);
	}

	void Remove(Cell* oldCell, Board board) {
		Figure figure = Figure();
		oldCell->Set(figure);
	}

	void Move(Figure figure, Cell* newCell, Cell* oldCell, Board board) {
		Remove(oldCell, board);
		Set(figure, newCell, board);
	}
};

class Players {
private:
	int size;
	std::vector<Player> players;
public:
	Players(int size_) : size(size_) {
		for (int i = 1; i <= size; i++) {
			players.push_back(Player(GetColor(i)));
		}
	}
	Player operator [] (int index) const {
		return players[index];
	}
};

//Класс GameRules
class GameRules {
public:
	GameRules() {}
};

//Класс Queens c наследованием класса GameRules для расстановки максимального количества небьющих друг друга ферзей
class Queens : public GameRules {
public:
	Queens(Board board_) :board(board_) {}
	int GetNumPlayers() {
		return numPlayers;
	}
	void Game(Players players, Board board_) {
		board = board_;
		CallPartingQueen(players[0], board.GetSize());
	}

private:
	Board board;
	Figure figureQueen = Figure('Q');
	int numPlayers = 1;

	//Функция для проверки клетки на наличие королевы на ней
	bool CheckHit(Cell cell) {
		return  cell.hit > 0;
	}

	//функция для проверки клетки на отсутствие королевы на ней
	bool CheckEmpty(Cell cell) {
		return cell == 'Z';
	}

	//Функция для перемещение королевы на другую клетку
	bool Set(Point PointQueen) {
		return !(CheckHit(board[PointQueen])) and board[PointQueen] == 'Z';
	}

	bool Remove(Point PointQueen) {
		return (board[PointQueen] == 'Q');
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
		if (point.GetColumn() >= 0 and point.GetColumn() < board.GetSize() and point.GetString() >= 0 and point.GetString() < board.GetSize()) {
			if (isHit) {
				board[point].hit += 1;
			}
			else {
				board[point].hit -= 1;
			}
		}
	}

	//Функция для перемещения по горизонтали
	void HorizonMoving(Point point, bool isHit) {
		for (int i = 0; i < board.GetSize(); i++) {
			Point point_ = Point(point.GetString(), i);
			if (board[point] != board[point_]) {
				FigureHit(point_, isHit);
			}
		}
	}

	//Функция для перемещения по вертикали
	void VerticalMoving(Point point, bool isHit) {
		for (int i = 0; i < board.GetSize(); i++) {
			Point point_ = Point(i, point.GetColumn());
			if (board[point] != board[point_]) {
				FigureHit(point_, isHit);
			}
		}
	}

	//Функция для перемещения по горизонтали
	void DiagonalMoving(Point point, bool isHit) {
		Point point_ = point;
		for (int i = 0; i < board.GetSize(); i++) {
			FigureHit(Point(point.GetString() + i, point.GetColumn() + i), isHit);
			FigureHit(Point(point.GetString() - i, point.GetColumn() + i), isHit);
			FigureHit(Point(point.GetString() + i, point.GetColumn() - i), isHit);
			FigureHit(Point(point.GetString() - i, point.GetColumn() - i), isHit);
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

	//Рекурсивная функция, которая ставит королеву и вызывает функцию для постановки следующей королевы
	Board PartingQueen(int numPlacedQueens, int numQueen, int column, int string, std::vector<Cell>* QueensInCell, Player player) {
		if (QueensInCell->size() == board.GetSize()) {
			return board;
		}
		for (int i = 0; i < board.GetSize(); i++) {
			if (CheckEmpty(board[string][i]) && CheckHit(board[string][i]) == false) {
				Point point = Point(string, i);
				Cell cell(Point(string, i));
				if (Set(point)) {
					player.Set(figureQueen, &board[point], board);
					HorizonMoving(point, true);
					VerticalMoving(point, true);
					DiagonalMoving(point, true);
					QueensInCell->push_back(cell);
				}
				if (string < board.GetSize()) {
					board = PartingQueen(numPlacedQueens, numQueen, 0, string + 1, QueensInCell, player);
					if (QueensInCell->size() == board.GetSize()) {
						return board;
					}
					if (Remove(point)) {
						player.Remove(&board[point], board);
						HorizonMoving(point, false);
						VerticalMoving(point, false);
						DiagonalMoving(point, false);
						QueensInCell->pop_back();
					}
				}
			}
		}
		return board;
	}
	//Функция, вызывающая реурсивную функцию. Возвращает доску с расставленными королевами
	Board CallPartingQueen(Player player, int numPlacedQueens) {
		std::vector<Cell>* QueensInCell = new std::vector<Cell>[board.GetSize()];
		board = PartingQueen(numPlacedQueens, 0, 0, 0, QueensInCell, player);
		std::cout << board;
		return board;
	}
};

//Шаблонный класс для игровой сессии
template <typename GameType>
class Match {
	GameType gr;
	Players players;
	Board board;
public:
	Match(GameType gr_, Players players_, Board board_) :gr(gr_), players(players_), board(board_) {}
	int Game(int size) {
		gr.Game(players, board);
		return 1;
	}
};

int main() {
	int a;
	std::cin >> a;
	Board board = Board(a);
	Queens q(board);
	Players players = Players(q.GetNumPlayers());
	Match<Queens> m(q, players, board);
	m.Game(9);
	return 1;
}