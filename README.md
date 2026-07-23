# Bus Reservation and Ticket Management System Using C++

## Project Title
SmartBus: Reservation & Ticket Management System

## Objective
Build a desktop application for bus reservation, booking, ticketing, payments and reporting with C++17, Qt 6 Widgets, SQLite, and CMake.

## Problem Statement
Managing bus reservations manually is error prone and inefficient. This system digitizes passenger registration, bus scheduling, route assignment, ticket booking, seat management, payment tracking, and reporting.

## Modules
1. Passenger, Bus and Route Management
2. Ticket Booking and Reservation Management
3. Payment, Ticket and Reporting

## Inputs and Outputs
- Inputs: passenger details, bus details, route details, booking requests, payment details.
- Outputs: booking confirmations, ticket text files, payment records, dashboard summaries, CSV reports.

## Technologies Used
- C++17
- Qt 6 Widgets
- Qt SQL module
- SQLite database
- CMake build system
- VS Code configuration

## C++ Concepts Demonstrated
- Classes and objects
- Encapsulation
- Constructors
- Inheritance
- Polymorphism
- STL containers like QVector and QMap
- Exception handling
- File handling
- Searching and sorting
- Header and implementation separation

## Database Structure
- `passengers`
- `buses`
- `routes`
- `bookings`
- `booked_seats`
- `payments`

Foreign keys connect passengers, buses, routes, bookings, and payments.

## Application Workflow
- Launch app -> database created if missing.
- Dashboard shows key metrics and recent activity.
- Manage passengers, buses, and routes.
- Book tickets, select seats, calculate fare.
- Handle reservations, cancelations, and payments.
- Export ticket text files and CSV reports.

## Dashboard Pages
- Dashboard
- Passengers
- Buses
- Routes
- Book Tickets
- Reservations
- Payments
- Reports

## Install Qt 6
1. Download Qt 6 from the official Qt website or install via Qt Maintenance Tool.
2. Ensure `qmake` and Qt directory are available.
3. Add Qt `bin` folder to your PATH.

## Configure CMake
From the project root run:
```powershell
mkdir build
cd build
cmake .. -G "Ninja" # or "Visual Studio 17 2022" if using MSVC
cmake --build . --config Release
```

## Build and Run Commands
Use the provided PowerShell script:
```powershell
.\build_and_run.ps1
```

If building manually:
```powershell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
.\Release\SmartBusReservationSystem.exe
```
