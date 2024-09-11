import pickle
from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)

# Load existing user data from file if available, otherwise initialize an empty dictionary
try:
    with open('users.pkl', 'rb') as f:
        users = pickle.load(f)
except FileNotFoundError:
    users = {}

@app.route('/')
def index():
    
    return render_template('index.html')

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        
        # Check if the username already exists
        if username in users:
            return 'Username already exists!'

        # Store the new user in the dictionary
        users[username] = password

        # Save the updated user data to file
        with open('users.pkl', 'wb') as f:
            pickle.dump(users, f)
        
        return redirect(url_for('login'))

    return render_template('signup.html')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        
        # Check if the username exists and the password is correct
        if username in users and users[username] == password:
            # Redirect to map.html if login successful
            return redirect(url_for('map'))
        
        return 'Login failed'

    return render_template('login.html')

@app.route('/map')
def map():
    return render_template('map.html')

if __name__ == '__main__':
    app.run(debug=True)
