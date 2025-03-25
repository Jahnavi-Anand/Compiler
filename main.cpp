#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

// Token structure
struct Token {
    string type;
    string value;
};

// Lexer: Converts source code into tokens
class Lexer {
    public:
        explicit Lexer(const string& input) : source(input), pos(0) {}
    
        vector<Token> tokenize() {
            vector<Token> tokens;
            while (pos < source.length()) {
                if (isspace(source[pos])) {
                    pos++;
                    continue;
                }
                // **Check keywords first**
                else if (source.substr(pos, 2) == "if" && !isalnum(source[pos + 2])) {
                    tokens.push_back({"IF", "if"});
                    pos += 2;
                }
                else if (source.substr(pos, 4) == "else" && !isalnum(source[pos + 4])) {
                    tokens.push_back({"ELSE", "else"});
                    pos += 4;
                }
                else if (source.substr(pos, 5) == "while" && !isalnum(source[pos + 5])) {
                    tokens.push_back({"WHILE", "while"});
                    pos += 5;
                }
                else if (source.substr(pos, 3) == "def" && !isalnum(source[pos + 3])) {
                    tokens.push_back({"DEF", "def"});
                    pos += 3;
                }
                else if (source.substr(pos, 6) == "return" && !isalnum(source[pos + 6])) {
                    tokens.push_back({"RETURN", "return"});
                    pos += 6;
                }
                else if (isalpha(source[pos]) || source[pos] == '_') {
                    tokens.push_back(identifier());
                }
                else if (isdigit(source[pos])) {
                    tokens.push_back(number());
                }
                else if (source[pos] == '+') {
                    tokens.push_back({"PLUS", "+"});
                    pos++;
                }
                else if (source[pos] == '-') {
                    tokens.push_back({"MINUS", "-"});
                    pos++;
                }
                else if (source.substr(pos, 2) == "<=") {  // Check for <= first
                    tokens.push_back({"LE", "<="});
                    pos += 2;
                }
                else if (source.substr(pos, 2) == ">=") {  // Check for >= first
                    tokens.push_back({"GE", ">="});
                    pos += 2;
                }
                else if (source[pos] == '<') {  // Then check for single <
                    tokens.push_back({"LT", "<"});
                    pos++;
                }
                else if (source[pos] == '>') {  // Then check for single >
                    tokens.push_back({"GT", ">"});
                    pos++;
                }
                else if (source.substr(pos, 2) == "==") {
                    tokens.push_back({"EQ", "=="});
                    pos += 2;
                }
                else if (source[pos] == '=') {  // Then check for single =
                    tokens.push_back({"ASSIGN", "="});
                    pos++;
                }
                else if (source[pos] == ':') {
                    tokens.push_back({"COLON", ":"});
                    pos++;
                }
                else if (source[pos] == '(') {
                    tokens.push_back({"LPAREN", "("});
                    pos++;
                }
                else if (source[pos] == ')') {
                    tokens.push_back({"RPAREN", ")"});
                    pos++;
                }
                else if (source[pos] == ',') {
                    tokens.push_back({"COMMA", ","});
                    pos++;
                }
                else if (source[pos] == '[') {
                    tokens.push_back({"LBRACKET", "["});
                    pos++;
                }
                else if (source[pos] == ']') {
                    tokens.push_back({"RBRACKET", "]"});
                    pos++;
                }
                else if (source[pos] == '*') {
                    tokens.push_back({"MULTIPLY", "*"});
                    pos++;
                }
                else if (source[pos] == '/') {
                    tokens.push_back({"DIVIDE", "/"});
                    pos++;
                }
                else {
                    cout << "Unknown character: " << source[pos] << endl;
                    exit(1);
                }
            }
            return tokens;
        }
    
    private:
        string source;
        size_t pos;
    
        Token identifier() {
            string result;
            while (pos < source.length() && (isalnum(source[pos]) || source[pos] == '_')) { // Allow underscores
                result += source[pos++];
            }
    
            // Check if it's a reserved keyword
            if (result == "if") return {"IF", result};
            if (result == "else") return {"ELSE", result};
            if (result == "while") return {"WHILE", result};
            if (result == "def") return {"DEF", result};
            if (result == "return") return {"RETURN", result};
    
            return {"IDENTIFIER", result}; // Otherwise, it's an identifier
        }
    
        Token number() {
            string result;
            while (pos < source.length() && isdigit(source[pos])) {
                result += source[pos++];
            }
            return {"NUMBER", result};
        }
    };

// AST Node
struct ASTNode {
    string type;
    string value;
    vector<ASTNode*> children; // For function arguments or body
    ASTNode* left;
    ASTNode* right;

    ASTNode(string type, string value = "", ASTNode* left = nullptr, ASTNode* right = nullptr)
        : type(type), value(value), left(left), right(right) {}

    // Helper function to add children
    void addChild(ASTNode* child) {
        children.push_back(child);
    }
};

// Parser: Converts tokens into AST
class Parser {
    public:
        explicit Parser(vector<Token> tokens) : tokens(move(tokens)), pos(0) {}
    
        vector<ASTNode*> parse() {
            vector<ASTNode*> statements;
            while (pos < tokens.size()) {
                statements.push_back(statement());
            }
            return statements;
        }
    
    private:
        vector<Token> tokens;
        size_t pos;
    
        ASTNode* term() {
            if (tokens[pos].type == "IDENTIFIER") {
                string identifierName = tokens[pos].value;
                pos++;
        
                // Check for array access
                if (pos < tokens.size() && tokens[pos].type == "LBRACKET") {
                    pos++; // Move past '['
                    ASTNode* index = expression(); // Parse the index
                    if (tokens[pos].type != "RBRACKET") {
                        cerr << "Syntax error: Expected ']' after array index\n";
                        exit(1);
                    }
                    pos++; // Move past ']'
                    return new ASTNode("ARRAY_ACCESS", identifierName, index);
                }
                // Check if it's a function call
                else if (pos < tokens.size() && tokens[pos].type == "LPAREN") {
                    pos--;
                    string functionName = tokens[pos++].value;
                    pos++; // Move past '('
                    // Parse arguments
                    ASTNode* argsNode = new ASTNode("ARGS", "");
                    while (tokens[pos].type != "RPAREN") {
                        argsNode->addChild(expression());
                        if (tokens[pos].type == "COMMA") {
                            pos++; // Move past ','
                        }
                    }
                    pos++; // Move past ')'
                    return new ASTNode("CALL", functionName, nullptr, argsNode);
                }
                else {
                    // Otherwise, it's a variable
                    return new ASTNode("IDENTIFIER", identifierName);
                }
            }
            else if (tokens[pos].type == "NUMBER") {
                return new ASTNode("NUMBER", tokens[pos++].value);
            }
            else if (tokens[pos].type == "LPAREN") {
                pos++; // Move past '('
                ASTNode* expr = expression();
                if (tokens[pos].type != "RPAREN") {
                    cerr << "Syntax error: Expected ')' after expression\n";
                    exit(1);
                }
                pos++; // Move past ')'
                return expr;
            }
            else {
                cerr << "Syntax error: Unexpected token " << tokens[pos].value << endl;
                exit(1);
            }
        }
    
        ASTNode* expression() {
            ASTNode* left = term();
    
            // Handle relational operators
            if (pos < tokens.size() && (tokens[pos].type == "LT" || tokens[pos].type == "GT" ||
                                        tokens[pos].type == "LE" || tokens[pos].type == "GE" ||
                                        tokens[pos].type == "EQ")) {
                string op = tokens[pos++].value;
                ASTNode* right = term();
                left = new ASTNode(op, "", left, right);
            }
    
            // Handle arithmetic expressions after relational operators (optional)
            while (pos < tokens.size() && (tokens[pos].type == "PLUS" || tokens[pos].type == "MINUS" ||
                                           tokens[pos].type == "MULTIPLY" || tokens[pos].type == "DIVIDE")) {
                string op = tokens[pos++].value;
                ASTNode* right = term();
                left = new ASTNode(op, "", left, right);
            }
    
            return left;
        }
    
        ASTNode* statement() {
            if (tokens[pos].type == "IF") {
                pos++; // Move past "if"
                ASTNode* condition = expression(); // Parse condition
                if (pos < tokens.size() && tokens[pos].type == "COLON") { 
                    pos++; // Move past ":"
                } else {
                    cerr << "Syntax error: Expected ':' after if condition, found '" 
                        << tokens[pos].value << "'\n";
                    exit(1);
                }
                ASTNode* ifBody = statement(); // Parse statement inside if
                ASTNode* elseBody = nullptr;
    
                if (pos < tokens.size() && tokens[pos].type == "ELSE") {
                    pos++; // Move past "else"
                    if (pos < tokens.size() && tokens[pos].type == "COLON") { // Check before accessing
                        pos++; // Move past ":"
                    } else {
                        cout << "DEBUG: Expected COLON, Found: " << tokens[pos].type << endl;
                        cout << "Syntax error: Expected ':' after else\n";
                        exit(1);
                    }
                    elseBody = statement();
                }
    
                return new ASTNode("IF", "", condition, new ASTNode("BLOCK", "", ifBody, elseBody));
            }
            else if (tokens[pos].type == "WHILE") {
                pos++; // Move past "while"
                ASTNode* condition = expression(); // Parse condition
                if (pos < tokens.size() && tokens[pos].type == "COLON") { 
                    pos++; // Move past ":"
                } else {
                    cerr << "Syntax error: Expected ':' after while condition, found '" 
                         << tokens[pos].value << "'\n";
                    exit(1);
                }
                ASTNode* body = statement(); // Parse statement inside while
                return new ASTNode("WHILE", "", condition, body);
            }
            else if (tokens[pos].type == "DEF") {
                pos++; // Move past "def"
                string functionName = tokens[pos++].value; // Function name
                if (tokens[pos].type != "LPAREN") {
                    cerr << "Syntax error: Expected '(' after function name\n";
                    exit(1);
                }
                pos++; // Move past '('
    
                // Parse parameters
                vector<string> parameters;
                while (tokens[pos].type != "RPAREN") {
                    if (tokens[pos].type == "IDENTIFIER") {
                        parameters.push_back(tokens[pos++].value);
                    } else if (tokens[pos].type == "COMMA") {
                        pos++; // Move past ','
                    } else {
                        cerr << "Syntax error: Expected parameter or ')'\n";
                        exit(1);
                    }
                }
                pos++; // Move past ')'
    
                if (tokens[pos].type != "COLON") {
                    cerr << "Syntax error: Expected ':' after function definition\n";
                    exit(1);
                }
                pos++; // Move past ':'
    
                // Parse function body (multiple statements)
                ASTNode* bodyNode = new ASTNode("BLOCK", "");
                while (pos < tokens.size() && tokens[pos].type != "DEF" && tokens[pos].type != "IDENTIFIER" && tokens[pos].type != "IF" && tokens[pos].type != "WHILE") {
                    bodyNode->addChild(statement());
                }
                return new ASTNode("FUNCTION", functionName, nullptr, bodyNode);
            }
            else if (tokens[pos].type == "RETURN") {
                pos++; // Move past "return"
                ASTNode* returnExpr = expression(); // Parse the return expression
                return new ASTNode("RETURN", "", returnExpr);
            }
            else if (tokens[pos].type == "IDENTIFIER" && tokens[pos + 1].type == "ASSIGN") {
                string varName = tokens[pos].value;
                pos += 2; // Move past the identifier and '='
                ASTNode* expr = expression(); // Parse the right-hand side of the assignment
                return new ASTNode("ASSIGN", varName, expr);
            }
            else if (tokens[pos].type == "IDENTIFIER" && tokens[pos + 1].type == "LBRACKET") {
                // Array assignment (e.g., arr[0] = 5)
                string arrayName = tokens[pos++].value;
                pos++; // Move past '['
                ASTNode* index = expression(); // Parse the index
                if (tokens[pos].type != "RBRACKET") {
                    cerr << "Syntax error: Expected ']' after array index\n";
                    exit(1);
                }
                pos++; // Move past ']'
    
                if (tokens[pos].type != "ASSIGN") {
                    cerr << "Syntax error: Expected '=' after array index\n";
                    exit(1);
                }
                pos++; // Move past '='
    
                ASTNode* value = expression(); // Parse the value to assign
                return new ASTNode("ARRAY_ASSIGN", arrayName, index, value);
            }
            else if (tokens[pos].type == "IDENTIFIER" && tokens[pos + 1].type == "LPAREN") {
                string functionName = tokens[pos++].value;
                pos++; // Move past '('
    
                // Parse arguments
                ASTNode* argsNode = new ASTNode("ARGS", "");
                while (tokens[pos].type != "RPAREN") {
                    argsNode->addChild(expression());
                    if (tokens[pos].type == "COMMA") {
                        pos++; // Move past ','
                    }
                }
                pos++; // Move past ')'
    
                return new ASTNode("CALL", functionName, nullptr, argsNode);
            }
    
            return expression();
        }
    };

// Code Generator: Converts AST to Assembly
class CodeGenerator {
public:
    map<string, string> symbolTable;

    void generateAssembly(vector<ASTNode*> statements, ofstream& outFile) {
        for (ASTNode* stmt : statements) {
            generate(stmt, outFile);
        }
    }

private:
void generate(ASTNode* node, ofstream& outFile) {
    static int labelCount = 0;

    if (node == nullptr) return;

    if (node->type == "ASSIGN") {
        generate(node->left, outFile);
        outFile << "POP " << node->value << endl;
    }
    else if (node->type == "ARRAY_ASSIGN") {
        // Generate code for the index
        generate(node->left, outFile);
        // Generate code for the value
        generate(node->right, outFile);
        outFile << "POP R1" << endl; // Value to assign
        outFile << "POP R2" << endl; // Index
        outFile << "STORE R1, " << node->value << "[R2]" << endl; // Store value at array[index]
    }
    else if (node->type == "ARRAY_ACCESS") {
        // Generate code for the index
        generate(node->left, outFile);
        outFile << "POP R1" << endl; // Index
        outFile << "LOAD R2, " << node->value << "[R1]" << endl; // Load value from array[index]
        outFile << "PUSH R2" << endl; // Push the value onto the stack
    }
    else if (node->type == "NUMBER" || node->type == "IDENTIFIER") {
        outFile << "PUSH " << node->value << endl;
    }
    else if (node->type == "+") {
        generate(node->left, outFile);
        generate(node->right, outFile), outFile;
        outFile << "POP R1\nPOP R2\nADD R1, R2\nPUSH R1" << endl;
    }
    else if (node->type == "-") {
        generate(node->left, outFile);
        generate(node->right, outFile);
        outFile << "POP R1\nPOP R2\nSUB R1, R2\nPUSH R1" << endl;
    }
    else if (node->type == "IF") {
        int currentLabel = labelCount++;
        generate(node->left, outFile);  // Condition evaluation

        outFile << "POP R1" << endl;
        outFile << "CMP R1, 0" << endl;
        outFile << "JZ ELSE_" << currentLabel << endl;

        generate(node->right->left, outFile);  // If body

        outFile << "JMP END_" << currentLabel << endl;
        outFile << "ELSE_" << currentLabel << ":" << endl;

        if (node->right->right) {
            generate(node->right->right, outFile);  // Else body
        }

        outFile << "END_" << currentLabel << ":" << endl;
    }
    else if (node->type == "WHILE") {
        int currentLabel = labelCount++;
        outFile << "LOOP_" << currentLabel << ":" << endl;

        generate(node->left, outFile);  // Condition evaluation

        outFile << "POP R1" << endl;
        outFile << "CMP R1, 0" << endl;
        outFile << "JZ END_" << currentLabel << endl;

        generate(node->right, outFile);  // Body of the while loop

        outFile << "JMP LOOP_" << currentLabel << endl;
        outFile << "END_" << currentLabel << ":" << endl;
    }
    else if (node->type == "FUNCTION") {
        outFile << node->value << ":" << endl; // Function label
        generate(node->right, outFile); // Generate function body
        outFile << "RET" << endl; // Return from function
    }
    else if (node->type == "CALL") {
        // Push arguments onto the stack
        for (ASTNode* arg : node->right->children) {
            generate(arg, outFile);
        }
        outFile << "CALL " << node->value << endl; // Call the function
        outFile << "POP R1" << endl; // Pop the return value into a register
        outFile << "PUSH R1" << endl; // Push the return value onto the stack
    }
    else if (node->type == "RETURN") {
        generate(node->left, outFile); // Generate code for the return expression
        outFile << "POP R1" << endl; // Pop the result into a register
        outFile << "RET" << endl; // Return from the function
    }
    else if (node->type == "BLOCK") {
        for (ASTNode* child : node->children) {
            generate(child, outFile);
        }
    }
    else if (node->type == "*") {
        generate(node->left, outFile);
        generate(node->right, outFile);
        outFile << "POP R1\nPOP R2\nMUL R1, R2\nPUSH R1" << endl;
    }
    else if (node->type == "/") {
        generate(node->left, outFile);
        generate(node->right, outFile);
        outFile << "POP R1\nPOP R2\nDIV R1, R2\nPUSH R1" << endl;
    }
}

};

// Main function
int main() {
    ifstream file("input.sl");
    if (!file) {
        cerr << "Error: Could not open input file.\n";
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();  // Read the entire file into a string
    string source = buffer.str();

    // Step 1: Tokenization
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << token.type << " -> " << token.value << endl;
    }

    // Step 2: Parsing
    Parser parser(tokens);
    vector<ASTNode*> ast = parser.parse();

    cout << "\nGenerated Assembly:\n";
    // Step 3: Code Generation
    ofstream outFile("output.asm");
    if (!outFile) {
        cerr << "Error: Could not open output file for writing.\n";
        return 1;
    }
    
    CodeGenerator generator;
    generator.generateAssembly(ast, outFile);

    outFile.close();

    outFile << "Assembly output written to output.asm\n";
    return 0;
}