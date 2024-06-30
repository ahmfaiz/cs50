import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)
app.secret_key = "StrongestKeyKnownToMankind"

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user = session["user_id"]
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as count FROM transactions WHERE user_id = ? GROUP BY symbol", user)
    total = 0

    if not stocks:
        return render_template("index.html")

    indexed = []

    for stock in stocks:
        if stock["count"] <= 0:
            continue
        price = lookup(stock["symbol"])["price"]
        total_price = price * stock["count"]
        stock["price"] = usd(price)
        stock["total_price"] = usd(total_price)
        total += total_price
        indexed.append(stock)

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user)
    cash = cash[0]["cash"]
    total += cash

    return render_template("index.html", stocks=indexed, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Validate form submission
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol not provided")

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing number of shares")

        try:
            shares = int(shares)
        except (ValueError):
            return apology("Invalid number of shares")

        if shares <= 0:
            return apology("Invalid number of shares")

        stock = lookup(symbol)

        if not stock:
            return apology("Stock does not exist")

        symbol = stock["symbol"]
        total_price = stock["price"] * shares

        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        balance = balance[0]["cash"]

        if balance < total_price:
            return apology("Insufficient balance")

        # Update user's cash balance and record the transaction
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_price, session["user_id"])
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, shares, total_price)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute(
        "SELECT symbol, shares, price, timestamp FROM transactions WHERE user_id = ?", session["user_id"])

    for element in history:
        element["price"] = usd(element["price"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")

        quotes = lookup(symbol)
        if not quotes:
            return apology("Symbol does not exist")

        quotes["price"] = usd(quotes["price"])

        return render_template("quoted.html", quotes=quotes)

    # Requested via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST request
    if request.method == "POST":

        entered_username = request.form.get("username")
        entered_password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not entered_username:
            return apology("must provide username")

        if not entered_password:
            return apology("must provide password")

        if not confirmation:
            return apology("must provide confirmation")

        if entered_password != request.form.get("confirmation"):
            return apology("Password and Confirmation do not match")

        hashed = generate_password_hash(entered_password)

        # Check if username already exists
        try:
            db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", entered_username, hashed)
        except (ValueError):
            return apology("Username already exists")

        flash("Registered Successfully!")
        return redirect("/login")

    # User reached route via GET request
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol not provided")

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing number of shares")

        try:
            shares = int(shares)
        except (ValueError):
            return apology("Invalid number of shares")

        if shares <= 0:
            return apology("Invalid number of shares")

        # Check if stock exists
        stock = lookup(symbol)
        if not stock:
            return apology("Stock does not exist")

        symbol = stock["symbol"]
        total_price = stock["price"] * shares

        # Check if user owns the amount of shares they wish to sell
        owns = db.execute(
            "SELECT SUM(shares) as count FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        owns = owns[0]["count"]
        if not owns or owns < shares:
            return apology("You poor. Shares to be sold exceed owned shares")

        # Update user's cash balance and record the transaction
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", total_price, session["user_id"])
        db.execute("INSERT INTO transactions(user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, -1 * shares,  total_price)

        return redirect("/")

    else:
        stocks = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
        owned = []
        for stock in stocks:
            owned.append(stock["symbol"])
        return render_template("sell.html", owned=owned)
