from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os
import traceback
import time

app = Flask(__name__)
CORS(app)

@app.route('/compile', methods=['POST'])
def compile_code():
    try:
        print("Received /compile request")
        data = request.get_json()
        code = data['code']
        print(f"Received code: {code}")

        with open('input.sl', 'w') as f:
            f.write(code)
        print("input file created")

        result = subprocess.run(['./a', 'input.sl', 'output.asm'], capture_output=True, text=True)
        print(f"compiler return code: {result.returncode}")

        if result.returncode != 0:
            print(f"compiler error: {result.stderr}")
            return jsonify({'error': result.stderr}), 500

        # Check for empty output.asm
        file_size = os.path.getsize('output.asm')
        if file_size == 0:
            print("Output file is empty. Waiting and checking again.")
            time.sleep(0.1)  # Wait for 100 milliseconds
            file_size = os.path.getsize('output.asm')
            if file_size == 0:
                print("Output file still empty. Returning error.")
                return jsonify({'error': "Assembly output file is empty or not generated."}), 500

        with open('output.asm', 'r') as f:
            assembly = f.read()
        print("output file read")
        return jsonify({'assembly': assembly})

    except Exception as e:
        print(f"Error in /compile: {e}")
        print(traceback.format_exc())
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)