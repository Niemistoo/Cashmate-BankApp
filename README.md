# SPO_Group17

### Team members:
- Sanna Koukkula
- Janne Niemi
- Eerika Pölkky
- Sanna Pöri-Tikkakoski

# ”CashMate” ATM Simulator App

## Introduction
The aim of this project was to make
an ATM (Automatic Teller Machine)
simulator app controlled only with a
pointing device, and to learn both
individual and group ICT project
management skills.

![Figure 1: Application main menu](/images/Cashmate_Usermenu.jpg)

## Objectives
The principal objective of the project
was to make a functioning Qt app
with dynamic libraries and
capabilities for logging in with a
single-account or combination
credit/debit card using an RFID
reader, and then viewing account
balances, viewing past account
transactions, and making
withdrawals (see Figure 1 above).
Secondary objectives included
hosting the app database and
backend code remotely, adding code
for window scalability, including a 
custom style sheet, and ensuring
consistent app behavior across both
Windows and macOS.

## Methods
A client-server architecture was used
to implement the application.
The client-side consists of a Qt
Widget Application and other
required components, such as
dynamic libraries and a custom
stylesheet. The server-side consists
of a remotely hosted node.js runtime
and REST API connected to a
remotely hosted MySQL database.
The database consists of several
parent-child relations, including
junction tables for enabling cards to
contain multiple accounts and for
users to have multiple cards (see
Figure 2 below).
System requirements for using the
app are the latest Windows or
macOS and a Qt Creator version
with serial port capabilities.

![Figure 2: MySQL database ER chart](/images/Cashmate_database.jpg)

## Results
The project works precisely as
intended. The user can log in by
scanning their card with an RFID
reader and inputting the card’s PIN.
If the card is a combination
credit/debit card, the user chooses
which they wish to use. The user can
then view their account balance,
credit limit, and funds available for
cash withdrawal, as well as view
past transactions in sets of 5 (see
Figure 3 below) and make both
fixed-amount and customized
withdrawals.

![Figure 3: View Transactions Menu](/images/Cashmate_Transactions.jpg)

## Conclusions
The project was a success. The
required workload was initially
overestimated, so the project group
was able to freely try additional
features and add or discard them as
wished. In retrospect, the teal
background with black text could
have been more high-contrast for
increased accessibility.