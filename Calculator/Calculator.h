#pragma once
#include <vector>
#include <exception>

class CalculatorException : public std::exception
{
public:
	enum ExceptionType  { None=0, DividedByZero };
	CalculatorException(std::string textWhat, ExceptionType exceptionType = None) {
		m_textWhat = textWhat;
		m_exceptionType = exceptionType;
	}
	
	virtual const char* what() const throw()
	{
		return m_textWhat.c_str();
	}
private:
	std::string m_textWhat;
	ExceptionType m_exceptionType;
};

class Calculator
{
public:
	// ActionType: то что было введено
	enum  ActionType {Number, Plus, Minus, Multiply, Divide, Equals, None};
	struct Action
	{
		ActionType actionType;
		double value;
	};

	void reset();
	bool addInput(const Action& input);
	Action getLastInput() const;
	// текущий (частичный) результат по мере ввода, который может быть рассчитан. 
	//“ермы результатом не €вл€ютс€
	// ѕринимать во внимание, пока терм не закончен: 3 + 2 х 5 возвратит 3 потому что
	// ¬ычисление терма не закончена.
	double getCurrentResult() const;
	bool hasLeftTermValue() const { return m_leftTerm.hasValue(); }
	bool hasLeftExpressionValue() const { return m_leftExpression.hasValue(); }
	bool isOperation(ActionType action) const;
	// actions
	int getActionsSize() { return static_cast<int>(m_actions.size()); }
	const Action& getAction(int i) { return m_actions.at(i);  }
private:
	bool isTerm(ActionType action) const;
	bool isExpression(ActionType action) const;
	ActionType getLastOperation();

// class:
	//  +,-
	class LeftExpression
	{
	public:
		LeftExpression(){m_hasValue = false; m_value = 0.0;}
		void reset();
		void set(double value);
		void add(double value);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
	private:
		bool m_hasValue;
		double m_value;
	};
	//  x,/
	class LeftTerm
	{
	public:
		LeftTerm(){m_hasValue = false;}
		void reset();
		void set(double value);
		void multiplyBy(double value);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
	private:
		bool m_hasValue;
		double m_value;
	};

// члены данных:
	std::vector<Action> m_actions; // все  действи€ пользователей которые были введены ( ActionType)
	// m_leftExpression всегда лева€ часть выражени€. ѕример:
	// 5 -> m_leftExpression = ActionType::None (reset) (*)
	// 5 + 3 + -> m_leftExpression = 8
	// 5 + 3 + 1 = -> m_leftExpression = 9
	// 5 + 3 + 1 = 9 + -> m_leftExpression = 9 (здесь после "=" ѕользователю Ётого класса необходимо добавить 9 перед 
    //использованием "+" потому что "=" "сбрасывает" 
	// и вычмислени€ должны быть начаты сначала,означа€ что вводитьс€ первое число).
	// 5 + 3 + 1 = 9 x -> m_leftExpression = ActionType::None (reset)
	
	LeftExpression m_leftExpression;
	// если вычислени€ начинаютс€ с термов:
	// 3 x 4, преобразуетс€ к  m_leftTerm (не в m_leftExpression).»так m_leftExpression
	// остаетс€ равным нулю до тех пор пока след. выр-е не будет введено.
	// 5 + 3 + 1 = 9 x -> m_leftTerm = 9 (m_leftExpression = ActionType::None)
	LeftTerm m_leftTerm;
};

inline void Calculator::LeftExpression::reset() 
{
	m_hasValue = false;
	m_value = 0.0;
}

inline void Calculator::LeftExpression::set(double value) 
{
	m_hasValue = true;
	m_value = value;
}

inline void Calculator::LeftExpression::add(double value)
{
	set(m_value + value);
}

inline void Calculator::LeftTerm::reset() 
{
	m_hasValue = false;
	m_value = 0.0;
}

inline void Calculator::LeftTerm::set(double value) 
{
	m_hasValue = true;
	m_value = value;
}

inline void Calculator::LeftTerm::multiplyBy(double value)
{
	set(m_value * value);
}