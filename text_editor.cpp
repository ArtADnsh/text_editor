#include <iostream>
#include <string>
#include <stack>
#include <sstream>

using namespace std;


struct Action {
    string type;
    int pos;
    string text;
};


int main() {
    string text = "";
    string clipboard = "";

    stack<Action> undoStack;
    stack<Action> redoStack;

    // Command-Line Help
    cout << "Simple Text Editor (CLI)\n";
    cout << "Commands:\n";
    cout << "  insert <pos> <text>\n";
    cout << "  delete <pos> <len>\n";
    cout << "  copy <pos> <len>\n";
    cout << "  paste <pos>\n";
    cout << "  undo\n";
    cout << "  redo\n";
    cout << "  print\n";
    cout << "  exit\n";

    string line;
    while (true) {
        cout << "\n> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            break;
        }
        else if (cmd == "print") {
            cout << text << "\n";
        }
        else if (cmd == "insert") {
            int pos;
            ss >> pos;

            string insertText;
            getline(ss, insertText);
            if (!insertText.empty() && insertText[0] == ' ')
                insertText.erase(0, 1);

            if (pos < 0 || pos > (int)text.size()) {
                cout << "Range Error\n";
                continue;
            }

            text.insert(pos, insertText);

            undoStack.push({"insert", pos, insertText});

            while (!redoStack.empty()) redoStack.pop();
        }
        else if (cmd == "delete") {
            int pos, len;
            ss >> pos >> len;

            if (pos < 0 || len < 0 || pos + len > (int)text.size()) {
                cout << "Range Error\n";
                continue;
            }

            string deleted = text.substr(pos, len);

            text.erase(pos, len);

            undoStack.push({"delete", pos, deleted});

            while (!redoStack.empty()) redoStack.pop();
        }
        else if (cmd == "copy") {
            int pos, len;
            ss >> pos >> len;

            if (pos < 0 || len < 0 || pos + len > (int)text.size()) {
                cout << "Range Error\n";
                continue;
            }

            clipboard = text.substr(pos, len);
            cout << "copied\n";
        }
        else if (cmd == "paste") {
            int pos;
            ss >> pos;

            if (pos < 0 || pos > (int)text.size()) {
                cout << "Range Error\n";
                continue;
            }

            if (clipboard.empty()) {
                cout << "Empty Error\n";
                continue;
            }

            text.insert(pos, clipboard);

            undoStack.push({"insert", pos, clipboard});

            while (!redoStack.empty()) redoStack.pop();
        }
        else if (cmd == "undo") {
            if (undoStack.empty()) {
                cout << "Empty Error\n";
                continue;
            }

            Action a = undoStack.top();
            undoStack.pop();

            if (a.type == "insert") {
                text.erase(a.pos, a.text.size());
            } 
            else {
                text.insert(a.pos, a.text);
            }

            redoStack.push(a);
            cout << "undo done\n";
        }
        else if (cmd == "redo") {
            if (redoStack.empty()) {
                cout << "Empty Error\n";
                continue;
            }

            Action a = redoStack.top();
            redoStack.pop();

            if (a.type == "insert") {
                text.insert(a.pos, a.text);
            }
            else {
                text.erase(a.pos, a.text.size());
            }

            undoStack.push(a);
            cout << "Redone\n";
        }
        else {
            cout << "Command Error\n";
        }
    }

}