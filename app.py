import sqlite3
from datetime import date
from pathlib import Path

import pandas as pd
import streamlit as st

DB = Path(__file__).with_name("smartbus_web.db")

st.set_page_config(page_title="SmartBus", page_icon="🚌", layout="wide")
st.markdown("""
<style>
.stApp{background:#f3f6fb;color:#10213b}
[data-testid="stSidebar"]{background:#0f1d36}
[data-testid="stSidebar"] *{color:white}
.card{background:white;border:1px solid #e2e8f0;border-radius:18px;padding:18px;min-height:115px;box-shadow:0 8px 24px rgba(15,23,42,.05)}
.label{font-size:14px;color:#64748b;margin-bottom:10px}
.value{font-size:30px;font-weight:800;color:#10213b}
input,textarea{color:#0f172a!important}
</style>
""", unsafe_allow_html=True)

def con():
    c = sqlite3.connect(DB, check_same_thread=False)
    c.execute("PRAGMA foreign_keys=ON")
    return c

def init():
    with con() as c:
        c.executescript("""
        CREATE TABLE IF NOT EXISTS passengers(
            passenger_id TEXT PRIMARY KEY,name TEXT NOT NULL,age INTEGER,gender TEXT,
            phone TEXT,email TEXT,address TEXT,proof_type TEXT,proof_number TEXT);
        CREATE TABLE IF NOT EXISTS buses(
            bus_id TEXT PRIMARY KEY,bus_number TEXT UNIQUE,bus_name TEXT,bus_type TEXT,
            total_seats INTEGER,driver_name TEXT,status TEXT);
        CREATE TABLE IF NOT EXISTS routes(
            route_id TEXT PRIMARY KEY,source TEXT,destination TEXT,departure TEXT,
            arrival TEXT,fare REAL,bus_id TEXT REFERENCES buses(bus_id));
        CREATE TABLE IF NOT EXISTS bookings(
            booking_id TEXT PRIMARY KEY,ticket_number TEXT UNIQUE,passenger_id TEXT REFERENCES passengers(passenger_id),
            route_id TEXT REFERENCES routes(route_id),bus_id TEXT REFERENCES buses(bus_id),
            travel_date TEXT,seat_number INTEGER,passengers INTEGER,total_fare REAL,
            booking_status TEXT,booking_date TEXT);
        CREATE TABLE IF NOT EXISTS payments(
            payment_id TEXT PRIMARY KEY,booking_id TEXT UNIQUE REFERENCES bookings(booking_id),
            amount REAL,payment_method TEXT,payment_status TEXT,payment_date TEXT);
        """)
        if c.execute("SELECT COUNT(*) FROM passengers").fetchone()[0] == 0:
            c.executemany("INSERT INTO passengers VALUES(?,?,?,?,?,?,?,?,?)",[
                ("P001","Thouhid Shaik",20,"Male","9876543210","thouhid@example.com","Tirupati","Aadhaar","XXXX1010"),
                ("P002","Aarav Kumar",24,"Male","9876500001","aarav@example.com","Chennai","Aadhaar","XXXX2020"),
                ("P003","Ananya Reddy",22,"Female","9876500002","ananya@example.com","Hyderabad","Passport","P30030"),
            ])
        if c.execute("SELECT COUNT(*) FROM buses").fetchone()[0] == 0:
            c.executemany("INSERT INTO buses VALUES(?,?,?,?,?,?,?)",[
                ("B001","AP03AB1234","Smart Express","Express",40,"Ramesh","Available"),
                ("B002","TN01CD5678","City Rider","Deluxe",36,"Suresh","Available"),
                ("B003","TS09EF9012","Royal Sleeper","AC Sleeper",32,"Kumar","Available"),
            ])
        if c.execute("SELECT COUNT(*) FROM routes").fetchone()[0] == 0:
            c.executemany("INSERT INTO routes VALUES(?,?,?,?,?,?,?)",[
                ("R001","Tirupati","Chennai","08:30","14:00",600,"B001"),
                ("R002","Chennai","Hyderabad","18:00","07:30",1200,"B003"),
                ("R003","Tirupati","Bangalore","07:00","13:30",850,"B002"),
            ])

def df(sql, params=()):
    with con() as c:
        return pd.read_sql_query(sql,c,params=params)

def newid(table,col,prefix,width=3):
    rows=df(f"SELECT {col} FROM {table}")[col].tolist()
    nums=[]
    for x in rows:
        try: nums.append(int(str(x).replace(prefix,"")))
        except: pass
    return f"{prefix}{max(nums,default=0)+1:0{width}d}"

def card(label,value,color="#10213b"):
    st.markdown(f'<div class="card"><div class="label">{label}</div><div class="value" style="color:{color}">{value}</div></div>',unsafe_allow_html=True)

init()

with st.sidebar:
    st.markdown("## 🚌 SmartBus")
    st.caption("Reservation & Ticket Management System")
    page=st.radio("Menu",["Dashboard","Passengers","Buses","Routes","Book Tickets","Reservations","Payments","Reports"],label_visibility="collapsed")

if page=="Dashboard":
    st.title("Dashboard")
    with con() as c:
        tb=c.execute("SELECT COUNT(*) FROM buses").fetchone()[0]
        tp=c.execute("SELECT COUNT(*) FROM passengers").fetchone()[0]
        today=c.execute("SELECT COUNT(*) FROM bookings WHERE travel_date=?",(date.today().isoformat(),)).fetchone()[0]
        seats=c.execute("SELECT COALESCE(SUM(total_seats),0) FROM buses").fetchone()[0]
        booked=c.execute("SELECT COUNT(*) FROM bookings WHERE booking_status IN('Confirmed','Pending')").fetchone()[0]
        cancelled=c.execute("SELECT COUNT(*) FROM bookings WHERE booking_status='Cancelled'").fetchone()[0]
        revenue=c.execute("SELECT COALESCE(SUM(amount),0) FROM payments WHERE payment_status='Paid'").fetchone()[0]
    cols=st.columns(6)
    vals=[("Total Buses",tb,"#2563eb"),("Total Passengers",tp,"#16a34a"),("Today's Bookings",today,"#f97316"),("Available Seats",max(seats-booked,0),"#0891b2"),("Cancelled Tickets",cancelled,"#dc2626"),("Total Revenue",f"₹{revenue:,.0f}","#2563eb")]
    for col,v in zip(cols,vals):
        with col: card(*v)
    st.subheader("Routes")
    st.dataframe(df("""SELECT r.route_id,b.bus_name,r.source||' → '||r.destination AS route,r.departure,r.arrival,r.fare,b.status
                     FROM routes r JOIN buses b ON b.bus_id=r.bus_id"""),use_container_width=True,hide_index=True)
    st.subheader("Recent Bookings")
    st.dataframe(df("""SELECT b.ticket_number,p.name,r.source||' → '||r.destination AS route,b.seat_number,b.booking_status
                     FROM bookings b JOIN passengers p ON p.passenger_id=b.passenger_id JOIN routes r ON r.route_id=b.route_id
                     ORDER BY b.rowid DESC LIMIT 10"""),use_container_width=True,hide_index=True)

elif page=="Passengers":
    st.title("Passenger Management")
    with st.form("pf",clear_on_submit=True):
        a,b,c=st.columns(3)
        with a:
            pid=st.text_input("Passenger ID",value=newid("passengers","passenger_id","P"),disabled=True)
            name=st.text_input("Name")
            age=st.number_input("Age",1,120,20)
        with b:
            gender=st.selectbox("Gender",["Male","Female","Other"])
            phone=st.text_input("Phone")
            email=st.text_input("Email")
        with c:
            address=st.text_input("Address")
            ptype=st.selectbox("Proof Type",["Aadhaar","Passport","Driving Licence","Other"])
            pnum=st.text_input("Proof Number")
        ok=st.form_submit_button("Add Passenger",use_container_width=True)
    if ok:
        if not name.strip() or not phone.isdigit():
            st.error("Enter a name and a valid numeric phone number.")
        else:
            with con() as c:
                c.execute("INSERT INTO passengers VALUES(?,?,?,?,?,?,?,?,?)",(pid,name,age,gender,phone,email,address,ptype,pnum))
            st.success(f"Passenger {pid} added.")
            st.rerun()
    st.dataframe(df("SELECT * FROM passengers ORDER BY passenger_id"),use_container_width=True,hide_index=True)

elif page=="Buses":
    st.title("Bus Management")
    with st.form("bf",clear_on_submit=True):
        a,b,c=st.columns(3)
        with a:
            bid=st.text_input("Bus ID",value=newid("buses","bus_id","B"),disabled=True)
            bnum=st.text_input("Bus Number")
            bname=st.text_input("Bus Name")
        with b:
            btype=st.selectbox("Bus Type",["Ordinary","Express","Deluxe","AC Sleeper","Non-AC Sleeper","Volvo"])
            total=st.number_input("Total Seats",1,80,40)
        with c:
            driver=st.text_input("Driver Name")
            status=st.selectbox("Status",["Available","On Trip","Maintenance","Inactive"])
        ok=st.form_submit_button("Add Bus",use_container_width=True)
    if ok:
        try:
            with con() as c:
                c.execute("INSERT INTO buses VALUES(?,?,?,?,?,?,?)",(bid,bnum,bname,btype,total,driver,status))
            st.success(f"Bus {bid} added."); st.rerun()
        except sqlite3.IntegrityError:
            st.error("Bus number already exists.")
    st.dataframe(df("SELECT * FROM buses ORDER BY bus_id"),use_container_width=True,hide_index=True)

elif page=="Routes":
    st.title("Route Management")
    buses=df("SELECT bus_id,bus_name FROM buses WHERE status!='Inactive'")
    opts={f"{r.bus_id} — {r.bus_name}":r.bus_id for r in buses.itertuples(index=False)}
    with st.form("rf",clear_on_submit=True):
        a,b,c=st.columns(3)
        with a:
            rid=st.text_input("Route ID",value=newid("routes","route_id","R"),disabled=True)
            source=st.text_input("Source")
            dest=st.text_input("Destination")
        with b:
            dep=st.time_input("Departure")
            arr=st.time_input("Arrival")
        with c:
            fare=st.number_input("Fare",0.0,value=500.0,step=50.0)
            buslabel=st.selectbox("Bus",list(opts))
        ok=st.form_submit_button("Add Route",use_container_width=True)
    if ok:
        if source.strip().lower()==dest.strip().lower() or not source.strip():
            st.error("Enter different source and destination.")
        else:
            with con() as c:
                c.execute("INSERT INTO routes VALUES(?,?,?,?,?,?,?)",(rid,source,dest,dep.strftime("%H:%M"),arr.strftime("%H:%M"),fare,opts[buslabel]))
            st.success(f"Route {rid} added."); st.rerun()
    st.dataframe(df("SELECT * FROM routes ORDER BY route_id"),use_container_width=True,hide_index=True)

elif page=="Book Tickets":
    st.title("Book Tickets")
    passengers=df("SELECT passenger_id,name FROM passengers")
    routes=df("""SELECT r.*,b.bus_name,b.bus_type,b.total_seats FROM routes r JOIN buses b ON b.bus_id=r.bus_id WHERE b.status='Available'""")
    pmap={f"{r.passenger_id} — {r.name}":r.passenger_id for r in passengers.itertuples(index=False)}
    rmap={f"{r.route_id} — {r.source} → {r.destination} — ₹{r.fare:.0f}":r.route_id for r in routes.itertuples(index=False)}
    if not pmap or not rmap:
        st.warning("Add a passenger and an available route first.")
    else:
        with st.form("book"):
            pl=st.selectbox("Passenger",list(pmap))
            rl=st.selectbox("Route",list(rmap))
            td=st.date_input("Travel Date",min_value=date.today())
            seat=st.number_input("Seat Number",1,80,1)
            count=st.number_input("Passengers",1,6,1)
            ok=st.form_submit_button("Confirm Booking",use_container_width=True)
        if ok:
            row=routes[routes.route_id==rmap[rl]].iloc[0]
            if seat>int(row.total_seats):
                st.error(f"Seat must be between 1 and {int(row.total_seats)}.")
            else:
                with con() as c:
                    used=c.execute("""SELECT COUNT(*) FROM bookings WHERE bus_id=? AND travel_date=? AND seat_number=? AND booking_status IN('Confirmed','Pending')""",(row.bus_id,td.isoformat(),seat)).fetchone()[0]
                if used:
                    st.error("Seat already booked.")
                else:
                    bk=newid("bookings","booking_id","BK")
                    tkt=newid("bookings","ticket_number","TKT",4)
                    pay=newid("payments","payment_id","PAY")
                    total=float(row.fare)*count
                    with con() as c:
                        c.execute("INSERT INTO bookings VALUES(?,?,?,?,?,?,?,?,?,?,?)",(bk,tkt,pmap[pl],row.route_id,row.bus_id,td.isoformat(),seat,count,total,"Confirmed",date.today().isoformat()))
                        c.execute("INSERT INTO payments VALUES(?,?,?,?,?,?)",(pay,bk,total,"UPI","Pending",date.today().isoformat()))
                    st.success(f"Booked successfully. Ticket {tkt}. Fare ₹{total:,.0f}")
                    st.rerun()
    st.dataframe(routes,use_container_width=True,hide_index=True)

elif page=="Reservations":
    st.title("Reservations")
    bookings=df("""SELECT b.booking_id,b.ticket_number,p.name,r.source||' → '||r.destination AS route,b.travel_date,b.seat_number,b.total_fare,b.booking_status
                   FROM bookings b JOIN passengers p ON p.passenger_id=b.passenger_id JOIN routes r ON r.route_id=b.route_id ORDER BY b.rowid DESC""")
    st.dataframe(bookings,use_container_width=True,hide_index=True)
    if not bookings.empty:
        bid=st.selectbox("Booking",bookings.booking_id.tolist())
        status=st.selectbox("New Status",["Confirmed","Pending","Completed","Cancelled"])
        if st.button("Update Status"):
            with con() as c: c.execute("UPDATE bookings SET booking_status=? WHERE booking_id=?",(status,bid))
            st.success("Status updated."); st.rerun()

elif page=="Payments":
    st.title("Payments")
    payments=df("""SELECT p.*,b.ticket_number FROM payments p JOIN bookings b ON b.booking_id=p.booking_id ORDER BY p.rowid DESC""")
    st.dataframe(payments,use_container_width=True,hide_index=True)
    if not payments.empty:
        pid=st.selectbox("Payment",payments.payment_id.tolist())
        status=st.selectbox("Status",["Paid","Pending","Failed","Refunded"])
        method=st.selectbox("Method",["UPI","Cash","Credit Card","Debit Card","Net Banking"])
        if st.button("Update Payment"):
            with con() as c: c.execute("UPDATE payments SET payment_status=?,payment_method=? WHERE payment_id=?",(status,method,pid))
            st.success("Payment updated."); st.rerun()

else:
    st.title("Reports")
    choice=st.selectbox("Report",["All Bookings","Confirmed","Cancelled","Passengers","Buses","Routes","Payments"])
    sql={
        "All Bookings":"SELECT * FROM bookings ORDER BY rowid DESC",
        "Confirmed":"SELECT * FROM bookings WHERE booking_status='Confirmed'",
        "Cancelled":"SELECT * FROM bookings WHERE booking_status='Cancelled'",
        "Passengers":"SELECT * FROM passengers",
        "Buses":"SELECT * FROM buses",
        "Routes":"SELECT * FROM routes",
        "Payments":"SELECT * FROM payments"
    }[choice]
    report=df(sql)
    st.dataframe(report,use_container_width=True,hide_index=True)
    st.download_button("Download CSV",report.to_csv(index=False).encode(),f"{choice.lower().replace(' ','_')}.csv","text/csv",use_container_width=True)
