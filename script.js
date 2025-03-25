function compileCode() {
    const inputCode = document.getElementById('input-code').value;

    fetch('http://127.0.0.1:5000/compile', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ code: inputCode }),
    })
    .then((response) => {
        console.log("Fetch response:", response);
        return response.json();
    })
    .then((data) => {
        console.log("Fetch data:", data);
        if (data.assembly) {
            document.getElementById('output-assembly').textContent = data.assembly;
        } else {
            document.getElementById('output-assembly').textContent = 'Error: ' + data.error;
        }
    })
    .catch((error) => {
        console.error("Fetch error:", error);
        document.getElementById('output-assembly').textContent = 'Error: ' + error;
    });
}

function switchTab(tabId) {
    const tabContents = document.querySelectorAll('.tab-content');
    const tabButtons = document.querySelectorAll('.tab-button');

    tabContents.forEach(tabContent => {
        tabContent.classList.remove('active');
    });

    tabButtons.forEach(tabButton => {
        tabButton.classList.remove('active');
    });

    document.getElementById(tabId).classList.add('active');
    event.target.classList.add('active');

    if (tabId === 'second-tab') {
        const pdfContainer = document.getElementById('second-tab');
        pdfContainer.innerHTML = `
  <div class="pdf-viewer-wrapper">
    <embed src="manual.pdf" type="application/pdf" class="pdf-embed">
  </div>
`;
    } else {
        const manualContainer = document.getElementById('second-tab');
        manualContainer.innerHTML = `
            <div class="manual-container">
                <h1>Simple Language Compiler Manual</h1>
                <p>Welcome to the working manual for our simple language compiler. This manual will guide you on how to write code and generate assembly output.</p>

                <h2>Language Features</h2>
                <ul>
                    <li><strong>Variables:</strong> Declare variables using identifiers (e.g., <code>a = 5</code>).</li>
                    <li><strong>Arithmetic Operations:</strong> Use <code>+</code>, <code>-</code>, <code>*</code>, <code>/</code> for calculations.</li>
                    <li><strong>Relational Operators:</strong> Use <code>&lt;</code>, <code>&gt;</code>, <code>&lt;=</code>, <code>&gt;=</code>, <code>==</code> for comparisons.</li>
                    <li><strong>Conditional Statements:</strong> Use <code>if</code> and <code>else</code> for branching (e.g., <code>if a &gt; 10: ... else: ...</code>).</li>
                    <li><strong>Loops:</strong> Use <code>while</code> for loops (e.g., <code>while i &lt; 5: ...</code>).</li>
                    <li><strong>Functions:</strong> Define functions using <code>def</code> and return values using <code>return</code>.</li>
                    <li><strong>Arrays:</strong> Access and assign values using <code>array[index]</code>.</li>
                    <li><strong>Function Calls:</strong> Call defined function with arguments.</li>
                </ul>

                <h2>Input Syntax</h2>
                <p>Write your code in the input text area. Each statement should be on a new line.</p>

                <h2>Example</h2>
                <pre>
                    def multiply(a, b):
                        return a * b

                    x = 5
                    if x > 0:
                        arr[0] = multiply(x, 2)
                    else:
                        arr[0] = 0
                </pre>

                <h2>Assembly Output</h2>
                <p>The generated assembly code will be displayed in the output text area. You can use this assembly code for further processing or analysis.</p>

                <h2>How It Works</h2>
                <p>The compiler works in three main stages:</p>
                <ol>
                    <li><strong>Lexical Analysis (Lexer):</strong> Converts the source code into a stream of tokens.</li>
                    <li><strong>Syntax Analysis (Parser):</strong> Builds an Abstract Syntax Tree (AST) from the tokens.</li>
                    <li><strong>Code Generation:</strong> Generates assembly code from the AST.</li>
                </ol>
            </div>
        `;
    }
}