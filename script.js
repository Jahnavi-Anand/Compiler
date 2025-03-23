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
    event.target.classList.add('active'); // Add active class to clicked button
}