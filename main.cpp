#include <cstring>
#include <iostream>
#include <list>
#include <vector>

using namespace std;


// lab02.cpp : Defines the entry point for the console application.
//
struct Symbol {
public:

    struct Expression {
    public:
        static char const A = 'A';
        static char const B = 'B';
        static char const C = 'C';
    };

    struct Operation {
    public:
        static char const Not = '!';
        static char const Disjunction = '|';
        static char const Junction = '&';
        static char const Implication = '>';
        static char const Equivalence = '=';
    };

    struct Bracket {
    public:
        static char const Open = '(';
        static char const Close = ')';
    };
};

// & | > =
int logicOperation(int a, int b, char op) {
    int result = 0;

    switch (op) {
        case Symbol::Operation::Junction:
            result = a & b;
            break;

        case Symbol::Operation::Disjunction:
            result = a | b;
            break;

        case Symbol::Operation::Equivalence:
            result = a == b;
            break;

        case Symbol::Operation::Implication:
            result = !(a == 1 && b == 0);
            break;

    }

    return result;
}


// a * (b * c)
int formula1(int a, int b, int c, char op1, char op2, bool nA, bool nB, bool nC, bool isPrintOut) {
    a = nA ? !a : a;
    b = nB ? !b : b;
    c = nC ? !c : c;

    int res1 = logicOperation(b, c, op2);
    if (isPrintOut)
        std::cout << res1;

    int res2 = logicOperation(a, res1, op1);

    return res2;
}

int formula1(int a, int b, int c, char op1, char op2, bool nA, bool nB, bool nC) {
    return formula1(a, b, c, op1, op2, nA, nB, nC, true);
}

// (a * b) * c
int formula2(int a, int b, int c, char op1, char op2, bool nA, bool nB, bool nC, bool isPrintOut) {
    a = nA ? !a : a;
    b = nB ? !b : b;
    c = nC ? !c : c;

    int res1 = logicOperation(a, b, op1);
    if (isPrintOut)
        std::cout << res1;

    int res2 = logicOperation(res1, c, op2);
    return res2;
}

int formula2(int a, int b, int c, char op1, char op2, bool nA, bool nB, bool nC) {
    return formula2(a, b, c, op1, op2, nA, nB, nC, true);
}

bool parseUserInput(const char *userInput,
                    bool &useFormula1,
                    int &a, int &b, int &c,
                    char &op1, char &op2,
                    bool &nA, bool &nB, bool &nC) {
    if (nullptr == userInput)
        return false;

    int len = strlen(userInput);

    if (len < 7)
        return false;

    int index = 0;
    char symbol = userInput[index];

    return true;
}

bool findChar(const char *input, char expression, int relPosition, char &foundChar) {
    const char *expressionPos = strchr(input, toupper(expression));

    if (NULL == expressionPos)
        return false;

    int pos = expressionPos + relPosition - input;

    if (pos < 0 || pos > strlen(input))
        return false;

    foundChar = input[pos];

    return true;
}

bool parseNotOperator(const char *input, char expression) {
    char foundChar = NULL;

    if (findChar(input, expression, -1, foundChar)) {
        if (Symbol::Operation::Not == foundChar) {
            return true;
        }
    }

    return false;
}

bool parseUseFormula2(const char *input) {
    return strchr(input, Symbol::Bracket::Open) == input;
}

bool calculateFormula(char *userInput, bool a, bool b, bool c) {

    bool notA = parseNotOperator(userInput, Symbol::Expression::A);
    bool notB = parseNotOperator(userInput, Symbol::Expression::B);
    bool notC = parseNotOperator(userInput, Symbol::Expression::C);

    bool useFormula2 = parseUseFormula2(userInput);

    char op1 = NULL;

    findChar(userInput, Symbol::Expression::A, 1, op1);

    char op2 = NULL;
    findChar(userInput, Symbol::Expression::B, useFormula2 ? 2 : 1, op2);


    int result = useFormula2 ? formula2(a, b, c, op1, op2, notA, notB, notC, false) : formula1(a, b, c, op1, op2,
                                                                                               notA, notB, notC, false);

    return result == 1;
}

int fractal(int n) {
    if (n <= 1)
        return 1;
    else
        return n * fractal(n - 1);
}

void generateVariant(vector<int> freeItem, vector<int> workItem, int **result, int &currentPos) {
    if (freeItem.size() == 0) {
        for (int i = 0; i < workItem.size(); i++) {
            result[currentPos][i] = workItem[i];
        }
        currentPos++;
        return;
    }
    for (int i = 0; i < freeItem.size(); i++) {
        vector<int> newSource;
        for (int j = 0; j < freeItem.size(); j++) {
            if (j == i)
                continue;
            newSource.push_back(freeItem[j]);
        }
        vector<int> newItem;
        for (int j = 0; j < workItem.size(); j++)
            newItem.push_back(workItem[j]);
        newItem.push_back(freeItem[i]);
        generateVariant(newSource, newItem, result, currentPos);
    }
}

int main(int argc, char *argv[]) {
    std::cout << "Not = !" << "\n";
    std::cout << "Disjunction = |" << "\n";
    std::cout << "Junction = &" << "\n";
    std::cout << "Implication = >" << "\n";
    std::cout << "Equivalence = '='" << "\n";

    int countFormuls;
    cout << "enter count of formuls ";
    int count;
//    cin >> count;
    count = 3;
    char (*userInputFormuls)[10] = new char[count][10];
//    for (int i = 0; i < count; i++) {
//        std::cout << "Type logical formular" << std::endl;
//        cin >> userInputFormuls[i];
//    }
    strcpy(userInputFormuls[0], "!A&(B|!C)");
    strcpy(userInputFormuls[1], "!A&(B|!C)");
    strcpy(userInputFormuls[2], "!A&(B|!C)");

    cout << endl;

    std::cout << Symbol::Expression::A << "\t";
    std::cout << Symbol::Expression::B << "\t";
    std::cout << Symbol::Expression::C << "\t";

    for (int i = 0; i < count; i++)
        cout << userInputFormuls[i] << "\t";

    cout << endl;

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {

            for (int c = 0; c <= 1; c++) {
                cout << a << "\t" << b << "\t" << c << "\t";
                for (int i = 0; i < count; i++)
                    cout << calculateFormula(userInputFormuls[i], a, b, c) << "\t";
                cout << endl;
            }
        }
    }
    int combinationCount = fractal(count);
    int **index = new int *[combinationCount];
    for (int i = 0; i < combinationCount; i++)
        index[i] = new int[count];
    vector<int> source;
    for (int i = 0; i < count; i++)
        source.push_back(i);
    int pos = 0;
    generateVariant(source, vector<int>(), index, pos);


    for (int i = 0; i < combinationCount; i++) {
        bool isImplication = true;
        for (int c = 0; c < count-1; c++) {
            for (int a = 0; a < 1; a++)
                for (int b = 0; b < 1; b++)
                    for (int d = 0; d < 1; d++) {
                        bool  result1 = calculateFormula(userInputFormuls[c],a,b,d);
                        bool  result2 = calculateFormula(userInputFormuls[c+1],a,b,d);
                        if(result1 && !result2)
                            isImplication = false;
                    }
        }
        if(isImplication){
            cout << endl;
            for (int c = 0; c < count; c++) {
                cout << "(" << userInputFormuls[c] << ")";
                if(c <count-1)
                    cout << "->";
            }
        }
    }


    getchar();
    system("pause");
    return 0;
}


