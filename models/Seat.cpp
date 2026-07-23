#include "Seat.h"

Seat::Seat()
{
}

Seat::Seat(int seatNumber, bool booked)
    : m_seatNumber(seatNumber), m_booked(booked)
{
}

int Seat::seatNumber() const
{
    return m_seatNumber;
}

bool Seat::isBooked() const
{
    return m_booked;
}

void Seat::setBooked(bool booked)
{
    m_booked = booked;
}
