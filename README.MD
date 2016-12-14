Про те як я міркував коли я писав

##Етап аналізу вихідної программи
* У нас є программа,  котра розраховує значення таблиці істиності,  нам потрібно ввести пару формул і аби вони обчислювались.Для цього потрібно винести все що відповідає за обчислення формули в корему функцію. Від чого ця формула повинна залежити? Від самої формули, котру потрібно обраховувати і значень a,b,c. Тому модифікуємо код,  створючи наступну функцію: 

```C++
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
```

Це скопійований код,  в котрому видалені допоміжні рядки. Тобто отримали функцію котра прийма логічний вираз як userInput і значення зміних і повертає результат підстановки цих змінних в программу. 

##Пошук алгоритму утворення комбінацій всіх формул
* Далі нам потрібно визначити функції котрі утворюють ланцюг із імплікацій. Для цього нам потрібно розглянути всі варіанти комбінацій і віднайти саме ті, котрі утворють потрібний нам ланцюжок,  але для цього нам потрібно розібратись яким саме чином отримати список всіх комбінцій. 

За це відповідає функція 
```C++
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
```

якщо абстрагуватись від реалізації то вона працює наступним чином:  береться список вихідних параметрів(список розміром Н). І з цього списку створюється Н нових списків.  Кожен з таких Н списків містить Н-ий елемент вихідного списку. Далі оброблюємо перший список.  Він містить тільки перший елемент вихідної послідовності,  тобто всі елементи що стоять після першого в нього не входять,  згрупуємо його в новий список розміром А (Х = Н-1). І знову проходимо по ньому циклом і сторіюємо А нових списків,  котрі будуть містити в собі А-ий елемент списку. Тобто ми отримаємему А списків кожен з яких буде містити перший елемент вихідного списку і А-ий нового. І так продовжуємо до тих пір,  поки "вільних" елементів не лишитья. (є малюнок як це відбувається)

Теперь пояснення стосовно функції. Вона приймає freeItem - список вільних елементів, workItem - список елементів котрі зафіксовані і являють собою частини вихідних списків, result - масив кобінацій і currentPos - місце в котре слід додавати нову комбінцію. 
При першому виклику функції freeItem містить всі елеемнти комбінції,  workItem - порожній, result порожній масив, але проініціалізований і currentPos = 0

vector - шаблонний тип данних(в трикутних скобках пишется тип данних котрий в нього можна зписувати) Цей тип данних схожий на масив з тією відмінністю що в нього можна динамічного додавати елементи, дізнаватись його розмір. 

Розглянемо строчку 
 generateVariant(newSource, newItem, result, currentPos);
функція виклика саму себе але модифікує параметри з newSource виключається i-тий елемент. Натомість він включається в кінець newItem.

Коли вільних параметрів не лишається (freeItem == 0) це значить що ми отримали нову комбінцію в змінній workItem і її потрібно перенести в масив відповідей(result). яке місце займе нова комбінація в архіві визначається змінною  currentPos. Оскільки це посилання - то модифікувавши його в одному з варіантів виклику функцій, воно автоматично змінится і в інших.

##Використання алгоритму комбінацій та вивід таблиці істиності 
* Ми маємо алгоритм для пошуку комбінцій,  повернемось до формул. Оскільки їх вже не одна а багато то їх ввід виконується наступним чином:

```C++
    cout << "enter count of formuls ";
    int count;
    cin >> count;
    char (*userInputFormuls)[10] = new char[count][10];
    for (int i = 0; i < count; i++) {
        std::cout << "Type logical formular" << std::endl;
        cin >> userInputFormuls[i];
    }
```
тобто массив userInputFormuls це массив строк. 

Далі відбувається ввивід таблиці істиності нових формул:
Шапка таблиці істиності:
```C++
    std::cout << Symbol::Expression::A << "\t";
    std::cout << Symbol::Expression::B << "\t";
    std::cout << Symbol::Expression::C << "\t";

    for (int i = 0; i < count; i++)
        cout << userInputFormuls[i] << "\t";

    cout << endl;
```

І власне сама таблиці істиності:
```C++
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
```

Наступний код шукає всі можливі комбінації формул: 
```C++
    int combinationCount = fractal(count);
    int **index = new int *[combinationCount];
    for (int i = 0; i < combinationCount; i++)
        index[i] = new int[count];
    vector<int> source;
    for (int i = 0; i < count; i++)
        source.push_back(i);
    int pos = 0;
    generateVariant(source, vector<int>(), index, pos);
```

combinationCount = кількість комбінацій (за законом комбінаторики їх факторіал від довжини вихідного списку)


В коді вище ми ініціалізуємо список вільних аргументів для функції generateVariant:
```C++
    vector<int> source;
    for (int i = 0; i < count; i++)
        source.push_back(i);
```

Тобто, для простоти ми шукаємо не комбінції власне формул а комбінції їх порядкових номерів. Тобто після виконання змінна індикс буде мати наступний вигляд у випадку, коли ми маємо 4 формули index ={{0,1,2,3},{0,1,3,2}, ... , {3,2,0,1},{3,2,1,0}} 

##Обробка утворених комбінацій,  пошук ланцюжка імплікацій
*Оскільки ми маємо список усіх можливіх комбінцій нам потрібно чи являються кожна з них ланцюжками імплікацій. Якщо це правда - то вивести їх на єкран. Тобто логічно що потрібно перебрати список комбінцій і потім в кожні комбінції перевірити чи наступна формула являє собою імплікіцію попередньої. 
Це робить настпуний код: 
```C++
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
```

перебір списку комбінацій:
```C++
for (int i = 0; i < combinationCount; i++) 
```

перебір елементів кожної і-тої комбінації: 
```C++
for (int c = 0; c < count-1; c++) 
```

Перебір кожноого з варіантів таблиці істиності 
```C++
for (int c = 0; c < count-1; c++) {
            for (int a = 0; a < 1; a++)
                for (int b = 0; b < 1; b++)
                    for (int d = 0; d < 1; d++) {
```

Далі обчислюємо значення функцій з данними змінними 
```C++
bool  result1 = calculateFormula(userInputFormuls[c],a,b,d);
bool  result2 = calculateFormula(userInputFormuls[c+1],a,b,d);
	if(result1 && !result2)
		isImplication = false;
```

для того аби визначити чи являється ланцюжок ланцюжком з імплікацій ми користуємось змінню isImplication котра за умовчанням правдива,  а у випадку коли імплікіція не виконується вона стає рівною false

Корисутючись змінною isImplication після перевірки всіх варіантів з таблиці істиности ми приймаємо рішення, чи слід виводити повідомлення про те,  що данний ланцюжок являє собою імплікацію:
```C++
if(isImplication){
            cout << endl;
            for (int c = 0; c < count; c++) {
                cout << "(" << userInputFormuls[c] << ")";
                if(c <count-1)
                    cout << "->";
            }
        }
```


