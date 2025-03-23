function compileCode() {
  const inputCode = document.getElementById('input-code').value;

  fetch('http://127.0.0.1:5000/compile', { // Corrected URL
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