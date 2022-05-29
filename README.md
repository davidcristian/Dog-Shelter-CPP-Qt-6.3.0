### Dog Shelter
This project is the GUI continuation of the console version found here: https://github.com/davidcristian/Dog-Shelter-CPP-Console

## 1
- Implement the interface design (location and size of GUI widgets, without attached functionalities), without using the Qt Designer.
- The list or table displaying the repository entities in administrator mode should be populated using an input file.

## 2
Add a filter for your repository and filter the elements according to any criteria you choose.
Please see an example below:
- The elements are filtered by the substring in the line edit box above the list.
- Hint: use the textChanged event of the QLineEdit.

## 3
-	All functionalities must be available through the GUI. You may use Qt Designer, if you want to change the initial design of your GUI.
-	The functionality of the application must be the same (including the one-by-one iteration of objects for the user mode).

## 4
- Add a gradient to one of the widgets on your interface (e.g. the list, a button, etc).
- Hint: see classes QLinearGradient and QPalette.

## 5
1. Add multiple *undo* and *redo* functionality for the `add`, `remove`, and `update` operations. Implement this functionality using inheritance and polymorphism. You will have **Undo** and **Redo** buttons on the GUI, as well as a key combination to undo and redo the operations (e.g. `Ctrl+Z`, `Ctrl+Y`).

2. Show the contents of the `adoption list` using a table view. You must use the [Qt View/Model](https://doc.qt.io/qt-5/modelview.html) components (`QTableView`). Create your own model – a class which inherits from [`QAbstractTableModel`](https://doc.qt.io/qt-5/qabstracttablemodel.html). This window will be opened from your GUI main window.

3. Add multiple *undo* and *redo* functionality for the `adoption list`. This will be tested through the application's GUI.

4. Use custom Qt delegates. Display an image of the dog in one of the columns of the Qt table view that shows the elements of the `adoption list`.
