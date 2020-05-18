// 김진구 2018203055

#include <iostream>
#include <stack>

using namespace std;

void read_and_evaluate(istream& ins);
void evaluate_stack(stack<double>& numbers, stack<char>& operations);

int main(void) {
	double result;
	while (true) {
		cout << ">> ";
		read_and_evaluate(cin); // 입력스트림이 인자이기 때문에
		cin.ignore();
	}
	return 0;
}

void read_and_evaluate(istream& ins) {
	const char DECIMAL = '.';

	stack<double> numbers; 
	stack<char> operations;
	double number;
	char symbol;
	bool first_input = true; // EOI가 입력되었을 때 EOI의 앞부분에 입력이 있는지 확인

	while (ins && ins.peek() != '\n') {
		if (ins.peek() == 'E' && first_input) {
			ins.ignore();
			if (ins.peek() == 'O') {
				ins.ignore();
				if (ins.peek() == 'I') {
					ins.ignore();
					if (ins.peek() == '\n') { // EOI의 뒷 부분에 다른 문자가 없는지 확인
						cout << "프로그램을 종료합니다." << endl;
						exit(1);
					}
				}
			}
		}
		first_input = false;
		if (isdigit(ins.peek()) || ins.peek() == DECIMAL) { 
			ins >> number;
			numbers.push(number);
		}
		else if (strchr("+-*/", ins.peek()) != NULL) {
			while (!operations.empty()) {
				if (strchr("+-", ins.peek()) != NULL && strchr("([{", operations.top()) == NULL) { 
					// 연산 우선순위에 따라 operations에 이미 값이 있다면 왼쪽괄호가 아닐경우 스택안의 값을 연산
					evaluate_stack(numbers, operations);
				}
				else {
					// */일 경우 */ 라면 스택안의 값을 연산
					if (strchr("*/", operations.top()) != NULL) {
						evaluate_stack(numbers, operations);
					}
					// +-와 왼쪽 괄호일 경우 연산을 먼저하지 않는다.
					else break;
				}
			}
			ins >> symbol;
			operations.push(symbol);
		}
		else if (strchr("([{", ins.peek()) != NULL) { // 왼쪽 괄호는 무조건 push
			ins >> symbol;
			operations.push(symbol);
		}
		else if (strchr(")]}", ins.peek()) != NULL) { 
			// 오른쪽괄호에서 왼쪽괄호가 나올 때 까지 혹은 operations가 비어있지 않을 때까지 연산한다
			ins >> symbol;
			while (!operations.empty() && strchr("([{", operations.top()) == NULL) {
				evaluate_stack(numbers, operations);
			}
			if (!operations.empty()) { 
				// 맞는 괄호가 오게 되면 pop하고 다른 괄호가 오면 
				// 에러메세지를 출력하고 오류로 인식하기 위해 값을 비워준다.
				switch (operations.top())
				{
				case '(':
					if (symbol == ')')
						operations.pop();
					else {
						cout << "Error!: unbalanced parentheses" << endl;
						while (!numbers.empty())
							numbers.pop();
						while (!operations.empty())
							operations.pop();
						while (ins.peek() != '\n')
							ins.ignore();
					}
					break;
				case '[':
					if (symbol == ']')
						operations.pop();
					else {
						cout << "Error!: unbalanced parentheses" << endl;
						while (!numbers.empty())
							numbers.pop();
						while (!operations.empty())
							operations.pop();
						while (ins.peek() != '\n')
							ins.ignore();
					}
					break;
				case '{':
					if (symbol == '}')
						operations.pop();
					else {
						cout << "Error!: unbalanced parentheses" << endl;
						while (!numbers.empty())
							numbers.pop();
						while (!operations.empty())
							operations.pop();
						while (ins.peek() != '\n')
							ins.ignore();
					}
					break;
				}
			}
			else { 
			// 끝까지 연산을 하였는데도 왼쪽괄호가 나오지 않는 경우 
			// 에러메세지를 출력하고 오류로 인식하기 위해 값을 비워준다.
				cout << "Error!: unbalanced parentheses" << endl;
				while (!numbers.empty())
					numbers.pop();
				while (!operations.empty())
					operations.pop();
				while (ins.peek() != '\n')
					ins.ignore();
			}

		}
		else {
			ins.ignore();
		}
	}

	while (!operations.empty()) {
		if (strchr("([{", operations.top()) != NULL) {
			// 왼쪽 괄호만 남아있는 경우 에러 메세지를 출력하고 오류로 인식하기 위해 값을 비워준다
			cout << "Error!: unbalanced parentheses" << endl;
			while (!numbers.empty())
				numbers.pop();
			while (!operations.empty())
				operations.pop();
			while (ins.peek() != '\n')
				ins.ignore();
		}
		else {
			// 사칙연산 기호가 남아있다면 비어있을 때 까지 연산한다
			evaluate_stack(numbers, operations);
		}

	}
	if (!numbers.empty()) {
		// 오류로 인식되어 값이 비어 있지 않다면 결과를 출력한다
		cout << numbers.top() << endl;
	}

}

void evaluate_stack(stack<double>& numbers, stack<char>& operations) {

	// numbers 스택에 값이 2개이상 존재하지 않으면 연산이 이루어지지 않으므로 
	// operations 스택을 비워주고 함수에서 나간다
	if (numbers.size() < 2) {
		while (!operations.empty())
			operations.pop();
		return;
	}

	double operand1, operand2;

	operand2 = numbers.top();
	numbers.pop();
	operand1 = numbers.top();
	numbers.pop();

	switch (operations.top()) { // 기호에 따라 사칙연산을 진행한다.
	case '+': numbers.push(operand1 + operand2); break;
	case '-': numbers.push(operand1 - operand2); break;
	case '*': numbers.push(operand1 * operand2); break;
	case '/':
		if (operand2 == 0) { 
			// 0으로 나누는 경우 에러메세지를 출력하고 오류로 인식하기 위해 값을 비워준다.
			cout << "Error!: divide by zero" << endl;
			while (!numbers.empty())
				numbers.pop();
		}
		else {
			numbers.push(operand1 / operand2);
		}
		break;
	}
	operations.pop();
}
