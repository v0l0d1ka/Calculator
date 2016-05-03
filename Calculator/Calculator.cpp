#include "stdafx.h"
#include "Calculator.h"

Calculator::Action Calculator::getLastInput() const 
{
	if(m_actions.size() <= 0){
		Action a;
		a.actionType = None;
		a.value = 0.0;
		return a;
	}else 
		return m_actions.back();
}

void Calculator::reset()
{
	m_leftExpression.reset();
	m_leftTerm.reset();
	m_actions.clear();
}

bool Calculator::isOperation(ActionType action) const
{
	return (action == ActionType::Plus || action == ActionType::Minus ||
		action == ActionType::Multiply || action == ActionType::Divide ||
		action == ActionType::Equals);
}

bool Calculator::isTerm(ActionType action) const
{
	return (action == ActionType::Multiply || action == ActionType::Divide);
}

bool Calculator::isExpression(ActionType action) const
{
	return (action == ActionType::Plus || action == ActionType::Minus);
}

Calculator::ActionType Calculator::getLastOperation()
{
	for (auto op = m_actions.rbegin(); op != m_actions.rend(); ++op)
	{
		if (isOperation(op->actionType))
			return op->actionType;
	}
	return ActionType::None;
}

double Calculator::getCurrentResult() const
{
	// Если "+" или "-" был введен, то эта функция всегда возвращает текущее
	// Значение m_leftExpression.
	// После того, как "=" m_leftExpression содержит текущий результат. 
	//Если после "="  ввода терма (и ни одно выражение не будет введено), 
	//то m_leftExpression сбрасывается и M_leftTerm содержит текущий результат.
	return m_leftExpression.hasValue() ? m_leftExpression.getValue() : m_leftTerm.getValue();
}

// Логика: 
// - Если это число,то просто добавить в вектор (но не допускается добавление числа два и более раз, 
//например "3 4 6 =" не корректный синтаксис. "3 + 6 =" корректный.)
// - Если это выражение-операция ("3+4-") тогда сперва выполнить операцию над выражением
// ("3+4") и добавить в "left expression" (m_leftExpression).
// -Если это операция-отношение (например 3+4x) добавим число слева (3) в m_leftExpression и
// число справа (4) добавим в  m_leftTerm.
// - "=" и "None" операции означают начало нового вычисления ,
// означает что первое число после присваивается "left expression" (m_leftExpression).
// После "=" и "None" первые временные результаты (до первой операции)
// относятся к  m_leftTerm, m_leftExpression равно нулю.
// возв. значение: Возв-ет true если ввод валиден,иначе false
bool Calculator::addInput(const Action& input)
{
	const Calculator::Action lastInput = getLastInput();

	if (input.actionType == ActionType::Number)
	{
		// добавление числа после числа выз. ошибку -> лишнее число игнорируется(3 4, 4 - игнор.)
		if (lastInput.actionType != ActionType::Number)
			m_actions.push_back(input);
	}
	else if (isOperation(input.actionType))
	{
		if (lastInput.actionType == ActionType::Number)
		{
			ActionType lastOperation = getLastOperation();
			switch (lastOperation)
			{
			case ActionType::Plus:
				if (isExpression(input.actionType) || input.actionType == ActionType::Equals)
				{
					// "3 + 4 -", "3 + 4 ="
					m_leftExpression.add(lastInput.value);
					m_leftTerm.reset();
				}
				else if (isTerm(input.actionType))
				{
					// "3 + 4 x",
					m_leftTerm.set(lastInput.value);
				}
				break;
			case ActionType::Minus:
				if (isExpression(input.actionType) || input.actionType == ActionType::Equals)
				{
					// "3 - 4 -", "3 - 4 ="
					m_leftExpression.add(-lastInput.value);
					m_leftTerm.reset();
				}
				else if (isTerm(input.actionType))
				{
					// "3 - 4 x",
					m_leftTerm.set(-lastInput.value);
				}
				break;
			case ActionType::Multiply:
				if (isExpression(input.actionType) || input.actionType == ActionType::Equals)
				{
					// "3 x 4 +", "3 x 4 ="
					m_leftExpression.add(m_leftTerm.getValue() * lastInput.value);
					m_leftTerm.reset();
				}
				else if (isTerm(input.actionType)) // "3 x 4 x"
					m_leftTerm.multiplyBy(lastInput.value);
				break;
			case ActionType::Divide:
				if (isExpression(input.actionType) || input.actionType == ActionType::Equals)
				{
					if (lastInput.value == 0.0)
					{
						CalculatorException divByZeroException("Error: Cannot Divide By Zero",
							CalculatorException::ExceptionType::DividedByZero);
						throw divByZeroException;
					}
					else
					{
						// "3 / 4 +", "3 / 4 ="
						m_leftExpression.add(m_leftTerm.getValue() / lastInput.value);
						m_leftTerm.reset();
					}
				}
				else if (isTerm(input.actionType)) // "3 / 4 x"
					m_leftTerm.multiplyBy(1.0 / lastInput.value);
				break;
			case ActionType::Equals: 
				if (isTerm(input.actionType))
				{
					// "= 3 x "
					m_leftExpression.reset();
					m_leftTerm.set(lastInput.value);
				}
				else if (isExpression(input.actionType))
				{
					// "= 3 + "
					m_leftExpression.set(lastInput.value);
					m_leftTerm.reset();
				}
				break;
			case ActionType::None: 
				if (isTerm(input.actionType))
				{
					// "3 x "
					m_leftExpression.reset();
					m_leftTerm.set(lastInput.value);
				}
				else if (isExpression(input.actionType))
				{
					// "3 + "
					m_leftExpression.set(lastInput.value);
					m_leftTerm.reset();
				}
				break;
			}
			m_actions.push_back(input);
			return true;
		}
	}
	return false;
}