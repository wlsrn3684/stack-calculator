// ������ 2018203055

#include <iostream>
#include <stack>

using namespace std;

void read_and_evaluate(istream& ins);
void evaluate_stack(stack<double>& numbers, stack<char>& operations);

int main(void) {
	double result;
	while (true) {
		cout << ">> ";
		read_and_evaluate(cin); // �Է½�Ʈ���� �����̱� ������
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
	bool first_input = true; // EOI�� �ԷµǾ��� �� EOI�� �պκп� �Է��� �ִ��� Ȯ��

	while (ins && ins.peek() != '\n') {
		if (ins.peek() == 'E' && first_input) {
			ins.ignore();
			if (ins.peek() == 'O') {
				ins.ignore();
				if (ins.peek() == 'I') {
					ins.ignore();
					if (ins.peek() == '\n') { // EOI�� �� �κп� �ٸ� ���ڰ� ������ Ȯ��
						cout << "���α׷��� �����մϴ�." << endl;
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
					// ���� �켱������ ���� operations�� �̹� ���� �ִٸ� ���ʰ�ȣ�� �ƴҰ�� ���þ��� ���� ����
					evaluate_stack(numbers, operations);
				}
				else {
					// */�� ��� */ ��� ���þ��� ���� ����
					if (strchr("*/", operations.top()) != NULL) {
						evaluate_stack(numbers, operations);
					}
					// +-�� ���� ��ȣ�� ��� ������ �������� �ʴ´�.
					else break;
				}
			}
			ins >> symbol;
			operations.push(symbol);
		}
		else if (strchr("([{", ins.peek()) != NULL) { // ���� ��ȣ�� ������ push
			ins >> symbol;
			operations.push(symbol);
		}
		else if (strchr(")]}", ins.peek()) != NULL) { 
			// �����ʰ�ȣ���� ���ʰ�ȣ�� ���� �� ���� Ȥ�� operations�� ������� ���� ������ �����Ѵ�
			ins >> symbol;
			while (!operations.empty() && strchr("([{", operations.top()) == NULL) {
				evaluate_stack(numbers, operations);
			}
			if (!operations.empty()) { 
				// �´� ��ȣ�� ���� �Ǹ� pop�ϰ� �ٸ� ��ȣ�� ���� 
				// �����޼����� ����ϰ� ������ �ν��ϱ� ���� ���� ����ش�.
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
			// ������ ������ �Ͽ��µ��� ���ʰ�ȣ�� ������ �ʴ� ��� 
			// �����޼����� ����ϰ� ������ �ν��ϱ� ���� ���� ����ش�.
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
			// ���� ��ȣ�� �����ִ� ��� ���� �޼����� ����ϰ� ������ �ν��ϱ� ���� ���� ����ش�
			cout << "Error!: unbalanced parentheses" << endl;
			while (!numbers.empty())
				numbers.pop();
			while (!operations.empty())
				operations.pop();
			while (ins.peek() != '\n')
				ins.ignore();
		}
		else {
			// ��Ģ���� ��ȣ�� �����ִٸ� ������� �� ���� �����Ѵ�
			evaluate_stack(numbers, operations);
		}

	}
	if (!numbers.empty()) {
		// ������ �νĵǾ� ���� ��� ���� �ʴٸ� ����� ����Ѵ�
		cout << numbers.top() << endl;
	}

}

void evaluate_stack(stack<double>& numbers, stack<char>& operations) {

	// numbers ���ÿ� ���� 2���̻� �������� ������ ������ �̷������ �����Ƿ� 
	// operations ������ ����ְ� �Լ����� ������
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

	switch (operations.top()) { // ��ȣ�� ���� ��Ģ������ �����Ѵ�.
	case '+': numbers.push(operand1 + operand2); break;
	case '-': numbers.push(operand1 - operand2); break;
	case '*': numbers.push(operand1 * operand2); break;
	case '/':
		if (operand2 == 0) { 
			// 0���� ������ ��� �����޼����� ����ϰ� ������ �ν��ϱ� ���� ���� ����ش�.
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
