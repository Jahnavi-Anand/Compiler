# Web-Based Simple Language Compiler IDE

This repository contains a web-based Integrated Development Environment (IDE) for a simple, custom programming language. It leverages Flask for the backend and JavaScript for the frontend to provide a user-friendly interface for writing and compiling code.

## Files

* `main.cpp`: The C++ compiler that converts the simple language code into assembly.
* `input.sl`: A sample input file (used for testing or examples).
* `api.py`: The Flask application that serves as the backend API for the IDE.
* `index.html`: The HTML file that defines the structure of the web-based IDE.
* `styles.css`: The CSS file that styles the web-based IDE.
* `script.js`: The JavaScript file that handles frontend interactions and API calls.
* `image.png`: background image for the IDE.

## Setup and Usage

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Jahnavi-Anand/Compiler.git
    cd compiler
    git checkout main
    ```

2.  **Install Flask and Flask-CORS:**
    ```bash
    pip install Flask flask-cors
    ```

3.  **Compile the C++ code:**
    ```bash
    g++ main.cpp -o a
    ```

4.  **Run the Flask application:**
    ```bash
    python api.py
    ```

5.  **Open the IDE:**
    * Open your web browser and navigate to `http://127.0.0.1:5000`.

## Features

* **Web-Based IDE:** A user-friendly interface for writing and compiling code directly in the browser.
* **Real-Time Compilation:** Code is compiled and assembly output is displayed instantly.
* **Working Manual:** An integrated manual that provides instructions on how to use the simple language.
* **Clear and Compile Buttons:** Easy-to-use buttons for clearing input/output and compiling code.
* **Styled Interface:** A visually appealing interface with a background image and custom styling.
* **GitHub Repository Link:** A link to the GitHub repository in the footer.

## Language Features

* **Variables:** Declare variables using identifiers (e.g., `a = 5`).
* **Arithmetic Operations:** Use `+`, `-`, `*`, `/` for calculations.
* **Relational Operators:** Use `<`, `>`, `<=`, `>=`, `==` for comparisons.
* **Conditional Statements:** Use `if` and `else` for branching (e.g., `if a > 10: ... else: ...`).
* **Loops:** Use `while` for loops (e.g., `while i < 5: ...`).
* **Functions:** Define functions using `def` and return values using `return`.
* **Arrays:** Access and assign values using `array[index]`.
* **Function Calls:** Call defined function with arguments.

## How It Works

1.  **Input:** The user writes code in the input text area of the web IDE.
2.  **Compilation Request:** When the user clicks the "Compile" button, JavaScript sends a POST request to the Flask backend (`api.py`).
3.  **Backend Processing:**
    * The Flask application receives the code, writes it to `input.sl`, and runs the compiled C++ executable (`./a`).
    * The C++ compiler generates assembly code and writes it to `output.asm`.
    * The Flask application reads the assembly code from `output.asm` and sends it back to the frontend.
4.  **Output:** JavaScript displays the assembly code in the output text area.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the [MIT License](LICENSE).