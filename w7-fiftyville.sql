-- Keep a log of any SQL queries you execute as you solve the mystery.

-- To list all tables
.tables

-- To get schema of crime_scene_reports table
.schema crime_scene_reports

-- To understand what the data in the table looks like
SELECT * FROM crime_scene_reports LIMIT 5;

-- To list the theft
SELECT * FROM crime_scene_reports WHERE year = 2023 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- The record says:
-- 1. Theft took place at 10:15 am at the Bakery.
-- 2. Witnesses mention the bakery.

-- Lets take a look at the interviews
.schema interviews
-- Selecting interviews given on the day mentioning bakery
SELECT transcript FROM interviews WHERE year = 2023 AND month = 7 and day = 28 AND transcript LIKE '%bakery%';

-- Notes about the thief based on witnesses:
-- 1. Drove a car out of the bakery parking around 10 mins later.
-- 2. Withdrew money from atm at Leggett Street before the theft.
-- 3. Thief called accomplice after leaving the bakery and talked to them for less than a minute.
-- 4. Thief took earliest flight out of fiftyville on 29.
---------------------------------------------------------


-- To understand bakery_security_logs table
.schema bakery_security_logs
SELECT * FROM bakery_security_logs LIMIT 5;

-- To list possible suspects based on timing of exit as per Note:1
SELECT license_plate FROM bakery_security_logs
WHERE
    day = 28 AND
    hour >= 10 AND
    minute >= 15 AND
    activity = 'exit' AND
    hour < 11 AND
    minute < 25;

-- Finding their ids and names
SELECT id, name FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND hour>=10 AND minute>=15 AND activity='exit' AND hour < 11 AND minute < 25);
---------------------------------------------------------


-- To understand atm_transactions table
.schema atm_transactions
SELECT * FROM atm_transactions LIMIT 5;

-- Selecting suspect account_numbers based on transactions as per Note:2
SELECT account_number FROM atm_transactions
WHERE atm_location = 'Leggett Street' AND transaction_type = 'withdraw' AND day = 28;

-- Finding person ids based on account numbers
.schema bank_accounts

SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE atm_location = 'Leggett Street' AND transaction_type = 'withdraw' AND day = 28);
---------------------------------------------------------


-- Understanding the phone_calls table
.schema phone_calls
SELECT * FROM phone_calls LIMIT 5;

-- Finding the suspect calls based on Note:3
SELECT caller FROM phone_calls WHERE day = 28 AND duration < 60;

-- Finding their ids
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND duration < 60);
---------------------------------------------------------


-- To understand the flights table
.schema flights

-- Filter flight the thief could have taken based on Note:4 (i.e. first flight from Fiftyville on 29th)
SELECT * FROM flights WHERE day = 29 AND origin_airport_id =
(SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER BY hour, minute LIMIT 1;

-- Understanding passengers table
.schema passengers
SELECT * FROM passengers LIMIT 5;

-- Listing ids of people who were aboard on the suspected flight
SELECT id FROM people WHERE passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id =
        (SELECT id FROM flights WHERE day = 29 AND origin_airport_id =
            (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY hour, minute LIMIT 1));
---------------------------------------------------------


-- FINDING INTERSECTION OF THESE 4 ID SETS TO HOPEFULLY FIND THE THIEF
SELECT id FROM people WHERE license_plate IN
    (SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND hour>=10 AND minute>=15 AND activity='exit' AND hour < 11 AND minute < 25)
INTERSECT
SELECT person_id FROM bank_accounts WHERE account_number IN
    (SELECT account_number FROM atm_transactions WHERE atm_location = 'Leggett Street' AND transaction_type = 'withdraw' AND day = 28)
INTERSECT
SELECT id FROM people WHERE phone_number IN
    (SELECT caller FROM phone_calls WHERE day = 28 AND duration < 60)
INTERSECT
SELECT id FROM people WHERE passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id =
        (SELECT id FROM flights WHERE day = 29 AND origin_airport_id =
            (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY hour, minute LIMIT 1));

-- Result:
-- +--------+
-- |   id   |
-- +--------+
-- | 686048 |
-- +--------+

-- thief is:
SELECT name FROM people WHERE id = 686048;
-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- +-------+

-- City he escaped to :
SELECT city FROM airports WHERE id =
    (SELECT destination_airport_id FROM flights WHERE day = 29 AND origin_airport_id =
        (SELECT id FROM airports WHERE city = 'Fiftyville')
    ORDER BY hour, minute LIMIT 1);
-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+

-- His accomplice
SELECT name FROM people WHERE phone_number =
    (SELECT receiver FROM phone_calls WHERE day = 28 AND caller =
        (SELECT phone_number FROM people WHERE name = 'Bruce'));
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+

