#ifndef SEAT_H
#define SEAT_H

class Seat
{
public:
    Seat();
    Seat(int seatNumber, bool booked = false);
    int seatNumber() const;
    bool isBooked() const;
    void setBooked(bool booked);

private:
    int m_seatNumber = 0;
    bool m_booked = false;
};

#endif // SEAT_H
