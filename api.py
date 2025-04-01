from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import subprocess
import os
import traceback
import json
import time
import re

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

        # Parse tokenization and AST output
        stdout_str = result.stdout.encode('utf-8', errors='replace').decode('utf-8') #add encoding.
        stdout_str = re.sub(r'[^\x20-\x7E\n]', '?', stdout_str) # replace non-printable characters.
        output_lines = stdout_str.splitlines()

        print("Raw stdout (encoded):", stdout_str) # debug

        try:
            tokens_start = output_lines.index("Tokens:") + 1
            print(f"Tokens start index: {tokens_start}") #debug
        except ValueError:
            print("Tokens: line not found in output")
            return jsonify({'error': "Tokens line not found in compiler output."}), 500

        try:
            ast_start = output_lines.index("AST:") + 1
        except ValueError:
            print("AST: line not found in output")
            return jsonify({'error': "AST line not found in compiler output."}), 500

        try:
            assembly_start = output_lines.index("Generated Assembly:") + 1
        except ValueError:
            print("Generated Assembly: line not found in output")
            return jsonify({'error': "Generated Assembly line not found in compiler output."}), 500

        tokens_data = []
        for line in output_lines[tokens_start:ast_start - 1]:
            line = line.strip().rstrip(',')
            if line:
                try:
                    #manual parse.
                    type_val = re.findall(r'"type":\s*"([^"]+)"', line)[0]
                    value_val = re.findall(r'"value":\s*"([^"]+)"', line)[0]
                    tokens_data.append({"type": type_val, "value": value_val})
                except Exception as e:
                    print(f"Error manual parsing line: {line}, error: {e}")
                    pass

        print(f"Tokens data: {tokens_data}") #debug
        print(f"Tokens data type: {type(tokens_data)}") #debug

        ast_data = "\n".join(output_lines[ast_start:assembly_start - 1])

        return jsonify({'assembly': assembly, 'lexerOutput': tokens_data, 'parserOutput': ast_data})

    except Exception as e:
        print(f"Error in /compile: {e}")
        print(traceback.format_exc())
        return jsonify({'error': str(e)}), 500

@app.route('/')
def serve_index():
    return send_from_directory('.', 'index.html')

@app.route('/<path:filename>')
def serve_static(filename):
    return send_from_directory('.', filename)

if __name__ == '__main__':
    app.run(debug=True)