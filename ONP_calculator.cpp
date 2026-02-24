// ONP_calculator.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//


#include <iostream>
#include <stdio.h>

typedef union Token {
    int operand;
    char function;
}token;

enum TokenType {
    Operand,
    Function
};

class Node {
public:
    Token token;
    int args;
    TokenType type;
    Node* next;

    Node(Token token, TokenType type) : token(token), type(type), next(nullptr), args(0) {};
};

class Stack {
private:
    Node* top;
public:
    Stack() :top(nullptr) {};

    bool isEmpty()const {
        return top == nullptr;
    }


    void put(Token token, TokenType type) {
        Node* new_node = new Node(token, type);
        if (isEmpty()) {
            new_node->next = NULL;
            top = new_node;
        }
        else {
            new_node->next = top;
            top = new_node;
        }
    }

    void pop() {
        if (!isEmpty()) {
            Node* new_node = top;
            top = top->next;
            delete new_node;
        }
        else {
            printf("stos pusty");
        }
    }

 

    void print() {
        Node* current = top;
        while (current != nullptr) {
            if (current->type == Operand) {
                printf("%d ", current->token);
            }
            else {
                printf("%c ", current->token);
            }
            
            current = current->next;
        }
        
    }

    Node* getTop() {
        return top;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

};

class Queue {
private:
    Node* First;
    Node* top;
public:
    Queue() :First(nullptr) {};

    bool isEmpty()const {
        return First == nullptr;
    }

    void put(Token token, TokenType type) {
        Node* new_node = new Node(token, type);
        if (isEmpty()) {
            new_node->next = NULL;
            First = new_node;
            top = new_node;
        }
        else {
            Node* last_node = top;
            last_node->next = new_node;
            top = new_node;     //w kolejce przechowany zostaje wskaznik na najwyszy element z powodow czasowych
        }
    }

    void pop() {
        if (!isEmpty()) {
            Node* new_node = First;
            First = First->next;
            delete new_node;
        }
        else {
            printf("kolejka pusta");
        }
    }

    void print() {
        Node* current = First;
        while (current != nullptr) {
            if (current->type == Operand) {
                printf("%d ", current->token);
            }
            else {
                if (current->token.function == 'X') {
                    printf("MAX%d ", current->args);
                }
                else if (current->token.function == 'M') {
                    printf("MIN%d ", current->args);
                }
                else if (current->token.function == 'I') {
                    printf("%cF ", current->token);
                }
                else {
                    printf("%c ", current->token);
                }
                
            }
            
            current = current->next;
            
        }
        
    }

    Node* getFirst() {
        return First;
    }
    Node* getLast() {
        return top;
    }

    ~Queue() {
        while (!isEmpty()) {
            pop();
        }
    }
};

void initBuffer(char buffer[12]) {  
    for (int i = 0; i < 12; i++) {
        buffer[i] = '\0';
    }

}

int getPriority(char function) {

    switch (function) {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case 'N':
        return 3;
    default:
        return 0;
    }

}

int do_calculations(char function,int a ,int b) {
    if (function == '+') {
        return a + b;
    }
    else if (function == '-') {
        return a - b;
    }
    else if (function == '*') {
        return a * b;
    }
    else if (function == '/') {
        if (b != 0) {
            return a / b;

        }
        else {
            std::cerr << "ERROR";
            return 0;
        }
    }
    else return 0;
}

void conversion(Stack& stack, Queue& onp) {
    Stack extras;
    char input;
    char buffer[12];
    int index = 0;
    initBuffer(buffer);
    int inside = 0;
    int commas = 0;
    int info = 0;
    int insideif = 0;
    int commasinsideofif = 0;
    while (1) {
        input = getchar();

        if (input == '.') {         //jesli input to kropka to koniec wpisywania zatem sciagamy wszystkie operatory ze stosu 
            while (!stack.isEmpty()) {              // i kladziemy je do kolejki
                Token item;
                item.function = stack.getTop()->token.function;
                
                onp.put(item, Function);
                if (item.function == 'X' || item.function == 'M') { //jesli operator to MIN lub MAX to sprawdzamy ilosc argumentow
                    onp.getLast()->args = stack.getTop()->args;
                }
                stack.pop();

            }
            break;

        }
        else if (input >= '0' && input <= '9') {    //jesli wykryto cyfre to wrzucamy ją do buffera
            buffer[index++] = input;
        }
        else {      //jesli input to nie liczba 
            if (buffer[0] != '\0') {    //jesli buffer nie jest pusty to przeksztalcamy liczbe za pomoca atoi i wrzucamy ja do kolejki
                Token a;
                a.operand = std::atoi(buffer);
                onp.put(a, Operand);
                initBuffer(buffer);
                index = 0;
            }
            else if (input==',') {  //obsluga zdarzenia gdy wykryto przecinek
                if (inside > 0) {   //jesli znajdujemy sie we wnetrzu funkcji MIN lub MAX to zwiekszamy nasza "flage" o 1
                    commas++;
                }
                if (insideif > 0 && stack.getTop()->token.function=='(') { 
                    commasinsideofif++;         //jesli znajdujemy sie we wnetrzu funkcji if to "to zwiekszamy flage zwiazana z IFem o 1"
                }
                
                if ( stack.getTop()->token.function != '(' && stack.getTop()->token.function != 'I' && stack.getTop()->token.function != 'N') {   
                    while (stack.getTop()->token.function != '(' && stack.getTop()->token.function != 'I' && stack.getTop()->token.function != 'N') {
                        Token IFoperator;
                        IFoperator.function = stack.getTop()->token.function;           //ta funkcja obsluguje przypadek w ktorym 
                        onp.put(IFoperator, Function);                                  //funkcja IF znajduje sie w funkcji MIN lub MAX
                        stack.pop();
                    }
                    info = 0; //operator odlozony na stos zatem zerujemy flage info
                    }

                if (!stack.isEmpty() && stack.getTop()->token.function == 'I') {    //jesli nastapil przecinek a najwyzszy element stosu to IF to sciagamy go
                    Token b;
                    b.function = stack.getTop()->token.function;
                    stack.pop();
                    onp.put(b, Function);
                }

                if (info && stack.getTop()->token.function!='(') { //jesli wykryto przecinek a na stos dopiero 
                    Token b;                                        //co odlozylismy jakis operator i jestesmy we wnetrzu MIN lub MAX to odkladamy operator do kolejki
                    b.function = stack.getTop()->token.function;
                    stack.pop();
                    onp.put(b, Function);                 
                    info = 0;
                }

                if (!stack.isEmpty() && stack.getTop()->token.function == 'N') {    //jesli wykryto przecinek a na stosie jest negacja to ja sciagamy
                    Token N;
                    N.function = 'N';
                    stack.pop();
                    onp.put(N, Function);
                }
            }
            else if(input=='M') {
                char input2 = getchar();
                char input3 = getchar();
                if (input2 == 'A' && input3 == 'X') {   //obsluga sytuacji gdy wpisany zostal operator MAX
                    if (inside > 0) {   //gdy jestesmy we wnetrzu MIN lub MAX to ilosc przecinkow wpisujemy na dodatkowy stos
                        extras.getTop()->args+=commas;  
                        commas = 0;
                    }
                    token max;
                    max.function = 'X';
                    stack.put(max, Function);
                    inside++;
                    extras.put(max, Function);  //odkladamy operator na dodatkowy stos i nadajemy mu poczatkowo ilosc argumentow=1
                    extras.getTop()->args = 1;
                }
                else {  //Obsluga zdarzenia gdy wpisany zostal operator MIN
                    if (inside > 0) {   //gdy jestesmy we wnetrzu MIN lub MAX to ilosc przecinkow wpisujemy na dodatkowy stos
                        extras.getTop()->args += commas;
                        commas = 0;
                    }
                    token min;
                    min.function = 'M';
                    stack.put(min,Function);
                    inside++;
                    extras.put(min, Function); //odkladamy operator na dodatkowy stos i nadajemy mu poczatkowo ilosc argumentow=1
                    extras.getTop()->args = 1;
                }
                
            }
            else if (input=='I') {  //obsluga zdarzenia gdy zostala wpisana funkcja IF
                char input2 = getchar();
                if (input2 == 'F') {
                    token IF;
                    IF.function = 'I';
                    stack.put(IF, Function);
                    insideif=1; //ustawiamy flage na 1
                    commasinsideofif = 0;

                    if (inside > 0) {   //w sytuacji gdy jestesmy we wnetrzu MIN lub MAX to musimy odjac dwa przecinki z funkcji MIN lub MAX poniewaz funkcja IF ma dwa przecinki
                        commas -= 2;
                    }
                }
            }
            
            else if (input == '(') {    //gdy zostal wpisany nawias to kladziemy go na stos
                token b;
                b.function = input;
                stack.put(b, Function);
            }
            else if (input == ')') {        //obsluga zdarzenia gdy uzytkownik zamknął nawias
                while (!stack.isEmpty()) {  //sciagamy wszystkie wyrazenia ze stosu dopoki nie bedzie pusty lub nie natrafimy na nawias otwierajacy (
                    char token = stack.getTop()->token.function;

                    if (token == '(') { //obsluga zdarzenia gdy natrafimy na nawias otwierajacy (

                        if (insideif > 0 && commasinsideofif == 2) {    //obsluga sytuacji w ktorej mozemy znalezc sie we wnetrzu funkcji IF
                            stack.pop();
                            Token IF;
                            IF.function = stack.getTop()->token.function;
                            if (IF.function == 'I') {       //jesli za nawiasem jest operator IF to kladziemy go do kolejki
                                onp.put(IF, Function);
                            }

                            if (stack.getTop()->token.function == 'M' || stack.getTop()->token.function == 'X') { //sytuacja w ktorej za nawiasem jest MIN lub MAX
                                stack.getTop()->args = extras.getTop()->args + commas;  //zczytujemy ilosc argumentow z naszego dodatkowego stosu
                                extras.pop();
                                inside--;   //zmniejszamy flage o jeden
                                commas = 0; 
                                Token extra;
                                extra.function = stack.getTop()->token.function;

                                onp.put(extra, Function);   //odkladamy nasza funkcje na stos i konczymy petle
                                onp.getLast()->args = stack.getTop()->args;
                                stack.pop();
                                break;
                            }

                            if (IF.function != 'N' && IF.function != '(' ) {    //jesli za nawiasem jest negacja to musimy ja zostawic poniewaz negacja jest right associative
                                stack.pop();
                            }

                            insideif = 0;   //zerujemy flagi
                            commasinsideofif = 0;
                            break;
                        }



                        if (inside > 0){    //obsluga sytuacji w ktorej jestesmy we wnetrzu MIN lub MAX
                            stack.pop();
                            if (stack.getTop()->token.function == '(' ) {
                                break;
                                
                            }
                            if (stack.getTop()->token.function == 'M' || stack.getTop()->token.function == 'X') {
                                stack.getTop()->args = extras.getTop()->args + commas; //zczytujemy ilosc argumentow z dodatkowego stosu
                                extras.pop();
                                inside--;
                                commas = 0;
                                Token extra;
                                extra.function = stack.getTop()->token.function;

                                onp.put(extra, Function);
                                onp.getLast()->args = stack.getTop()->args;
                                stack.pop();
                            }

                            break;
                        }
       
                        stack.pop();    //usuwamy zczytany token czyli ( i obslugujemy inne przypadki
                        
                        
                        if (!stack.isEmpty() && stack.getTop()->token.function == 'N') { //jesli za nawiasem jest negacja to odkladamy ja na do kolejki
                            Token N;
                            N.function = 'N';
                            onp.put(N, Function);
                            stack.pop();
                            break;
                        }
                        if (!stack.isEmpty() && stack.getTop()->token.function == 'I') { //jesli za nawiasem byl IF to odkladamy ja do kolejki
                            Token I;
                            I.function = 'I';
                            onp.put(I, Function);
                            stack.pop();
                            break;
                        }

 
                        break;
                    }
                    else { //obsluga sytuacji w ktorej znak nie nawiasem
                        if (stack.getTop()->type == Operand) {  //jesli znak jest liczba to odkladamy go do kolejki
                            Token b;
                            b.operand = stack.getTop()->token.operand;
                            onp.put(b, Operand);
                            
                        }
                        else {  //w przeciwnym wypadku na stosie byl operator zatem odkladamy go do kolejki
                            Token b;
                            b.function = stack.getTop()->token.function;
                            onp.put(b, Function);
                            if (b.function == 'X' || b.function == 'M') { //jesli operator to MAX lub MIN to przypisujemy mu ilosc argumentow
                                onp.getLast()->args = stack.getTop()->args;
                            }
                        }


                    }
                    stack.pop();


                }
            }
            else {      //obsluga sytuacji w ktorej  uzytkownik wprowadzil "zwykly" operator typu + - / * N
                if (input == ' ') {
                    continue;
                }
                int priority = getPriority(input); //sprawdzamy priorytet
                while (!stack.isEmpty()) { //sciagamy operatory ze stosu do momentu gdy bedzie on pusty lub trafimy na nawias lub mniejszy priorytet
                    char top = stack.getTop()->token.function;
                    stack.pop();
                    token b;
                    b.function = top;
                    if (top=='N' && input=='N') { //jesli na stosie jest negacja to odkladamy wpisany znak na stos i przerywamy petle
                        stack.put(b, Function);
                        break;
                    }
                    else if (top == '(' || getPriority(top) < priority) { //jesli trafimy na nawias lub mniejszy priorytet znaku to odkladamy operator na stos i konczymy petle

                        stack.put(b, Function);
                        if (inside > 0) { 
                            info = 1; //flaga mowiaca nam ze na stos odlozylismy operator, ktory znajduje sie wewnatrz funkcji MIN lub MAX
                        }
                        break;
                    }
                    else {  //jesli operator na stosie ma wiekszy priorytet to odkladamy go do kolejki
                        
                        onp.put(b, Function); 
                    }

                }
                token c;
                if (!isspace(input)) { //po skonczonym sciaganiu operatorow jestli takowe byly, odkladamy wpisany znak przez uzytkownika na stos
                    c.function = input;
                    stack.put(c, Function);
                }
                
                

            }

        }
    }

}

void calculation(Stack& stack, Queue& onp) {
    while (!onp.isEmpty()) {    

        if (onp.getFirst()->type == Operand) {  //jesli trafiamy na liczbe to sciagamy ja z onp i wrzucamy na stos
            Token a;
            a.operand = onp.getFirst()->token.operand;
            stack.put(a, Operand);
            onp.pop();
           
            
        }
        else { 
            char function;
            function = onp.getFirst()->token.function;
            
           
            if (function == 'X') {  //obsluga sytuacji gdy funkcja to MAX
                int args = onp.getFirst()->args;    //zczytujemy ilosc argumentow
                onp.pop();
                printf("MAX%d ", args);
                stack.print();
                printf("\n");
                int max = stack.getTop()->token.operand;
                stack.pop();
                for (int i = 0; i < args - 1; i++) {    //na stosie szukamy najwiekszej liczby posrod argumentow funkcji
                    if (max < stack.getTop()->token.operand) {
                        max = stack.getTop()->token.operand;
                    }
                    stack.pop();
                }
                Token r;
                r.operand = max;
                stack.put(r, Operand);
            }
            else if (function == 'M') { //obsluga sytuacji gdy funkcja to MIN
                int args = onp.getFirst()->args;     //zczytujemy ilosc argumentow
                onp.pop();
                printf("MIN%d ", args);
                stack.print();
                printf("\n");
                int min = stack.getTop()->token.operand;
                stack.pop();
                for (int i = 0; i < args - 1; i++) {    //szukamy najmniejszej liczby
                    if (min > stack.getTop()->token.operand) {
                        min = stack.getTop()->token.operand;
                    }
                    stack.pop();
                }
                Token r;
                r.operand = min;
                stack.put(r, Operand);
            }
            else if (function == 'I') {     //obsluga sytuacji gdy funkcja to IF
                onp.pop();
                printf("%cF ", function);
                stack.print();
                printf("\n");
                int op3, op2, op1;

                op3 = stack.getTop()->token.operand;
                stack.pop();
                op2 = stack.getTop()->token.operand;
                stack.pop();
                op1 = stack.getTop()->token.operand;
                
                
                stack.pop();

                int result = (op1 > 0) ? op2 : op3;     //wykonujemy operacje, jesli pierwszy operator > 0 to zwracamy drugi operator a w przeciwnym wypadku trzeci operator

                Token r;
                r.operand = result;
                stack.put(r, Operand);
            } 
            else if (function == 'N') { //obsluga sytuacji gdy funkcja to negacja
                onp.pop();
                printf("%c ", function);
                stack.print();
                printf("\n");

                int op = stack.getTop()->token.operand;
                stack.pop();

                int result = op * (-1); //wykonujemy dzialanie
                Token r;
                r.operand = result;
                stack.put(r, Operand);
            }else{ //obsluga sytuacji gdy funkcja to + - / *
                onp.pop();
                printf("%c ", function);
                stack.print();
                printf("\n");
                int op2, op1;
                if (stack.isEmpty()) { //ten fragment kodu jest niepotrzebny ale gdyby nie on to w tescie 7 i 10 bylby acces violation zamiast zlej odpowiedzi
                    return;
                }

                op2 = stack.getTop()->token.operand;
                stack.pop();
                if (stack.isEmpty()) { //ten fragment kodu jest niepotrzebny ale gdyby nie on to w tescie 7 i 10 bylby acces violation zamiast zlej odpowiedzi
                    return;
                }
                op1 = stack.getTop()->token.operand;
                
                stack.pop();

                if (op2 == 0 && function == '/') {  //jesli probujemy dzielic przez zero to wypisujemy error i usuwamy wszystko ze stosu i kolejki
                    printf("ERROR\n");
                    while (!stack.isEmpty()) {
                        stack.pop();
                    }
                    while (!onp.isEmpty()) {
                        onp.pop();
                    }
                    return;
                }

                int result = do_calculations(function, op1, op2); //wykonujemy kalkulacje
                Token r;
                r.operand = result;
                stack.put(r, Operand);
                }
            
            

        }
    }

}



int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    Stack stack;
    Queue onp;
    int numbers;
    
    scanf("%d", &numbers);

    for (int z = 0; z < numbers; z++) {
        conversion(stack, onp);
        onp.print();
        printf("\n");
        calculation(stack,onp);
        if (!stack.isEmpty()) {
            stack.print();
        }
        printf("\n");
        if (!stack.isEmpty()) {
            stack.pop();
        }
    }
    return 0;
}
