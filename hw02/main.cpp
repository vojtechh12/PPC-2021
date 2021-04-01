#include <iostream>
#include <vector>
#include <iomanip>
#include "doprava.hpp"

//Feel free to create your solution in multiple files
// and add the includes here. e.g.:
//#include "solution.hpp"

using namespace std;



void print_timetable(Network net, string iter) 
{
    Line tmpL;
    Time depT;
    vector<vector<Depart>> st_depf, st_depb;
    vector<Depart> hr_depf, hr_depb;
    size_t hr = 0;
    int s, m, h, z; //seconds, minutes, hours

    for (int i = 0; i < net.nlines(); i++) {
        tmpL = net.getLine(i);
        st_depf.clear();
        hr_depf.clear();
        st_depb.clear();
        hr_depb.clear();

        for (size_t st = 0; st < tmpL.stops.size(); st++) {
            
            if (tmpL.stops[st] == iter) {
                // print the timetable for Line tmpL at station iter
                // load the right side of the timetable
                {
                hr = 0;
                for (auto conn : tmpL.conns_fwd) {
                    depT = conn[st].ti;
                    Depart dpr;
                    dpr.dir = Lndir::fwd;
                    dpr.ti = depT;
                    depT.gett(h, m, s);
                    if (h == (int)hr) {
                        hr_depf.push_back(dpr);
                    }
                    else {
                        while (h != (int)hr) {
                        hr++;
                        st_depf.push_back(hr_depf);
                        }
                        hr_depf.clear();
                        hr_depf.push_back(dpr);
                    }
                }
                st_depf.push_back(hr_depf);
                // load the blanks for more convenient printing
                while (hr < 23) {
                    hr++;
                    hr_depf.clear();
                    st_depf.push_back(hr_depf);
                }
                
                // load the right side of the table
                hr = 0;
                for (auto conn : tmpL.conns_bwd) {
                    depT = conn[st].ti;
                    Depart dpr;
                    dpr.dir = Lndir::bwd;
                    dpr.ti = depT;
                    depT.gett(h, m, s);
                    if (h == (int)hr) {
                        hr_depb.push_back(dpr);
                    }
                    else {
                        while (h != (int)hr) 
                        {
                        hr++;
                        st_depb.push_back(hr_depb);
                        }
                        hr_depb.clear();
                        hr_depb.push_back(dpr);
                    }
                }
                st_depb.push_back(hr_depb);
                // load the blanks for more convenient printing
                while (hr < 23) {
                    hr++;
                    hr_depb.clear();
                    st_depb.push_back(hr_depb);
                    }
                }
                
                
                // now I have to print the table for this line (i) at station st
                // header starts here
                cout << '+'<<string(78, '-')<<'+'<<endl;
                cout << "| "<<tmpL.stops[st]<<string(69 - tmpL.stops[st].size(), ' ')<<"Line: "<<i<<" |"<<endl;
                cout << '+'<<string(38,'-')<<"++"<<string(38,'-')<<'+'<<endl;
                cout << "| To: "<<*tmpL.stops.rbegin()<<string(33- (*tmpL.stops.rbegin()).size(), ' ');
                cout << "|| To: "<<*tmpL.stops.begin()<<string(33- (*tmpL.stops.begin()).size(), ' ')<<'|'<<endl;
                cout << "+----+"<<string(33, '-')<<"++----+"<<string(33,'-')<<'+'<<endl;
                // end of the header
                // here begins the table with departures
                for (hr = 0; hr < st_depf.size(); hr++) {
                    z = 0;
                    cout << "| "<<setw(2) << setfill('0') << hr << " | ";
                    for (auto d : st_depf[hr]) {
                        depT = d.ti;
                        depT.gett(h, m, s);
                        cout << setw(2) << setfill ('0') << m << ' ';
                        
                        z+=3;
                    }
                    // print the right side
                    
                    cout << string((32-z), ' ');
                    z = 0;
                    cout<< "|| "<<setw(2)<<setfill('0')<<hr<<" | ";
                    for (auto d : st_depb[hr]) {
                        depT = d.ti;
                        depT.gett(h, m, s);
                        cout << setw(2) << setfill ('0') << m << ' ';
                        z+=3;
                        
                    }
                    cout << string((32-z), ' ')<<'|'<<endl;
                }
                cout << "+----+"<<string(33, '-')<<"++----+"<<string(33,'-')<<'+'<<endl;
            }
        }
        
    }
}

void get_d (Network net , vector<int> & df, vector<int> &db, int l) {
    // d - difference, l - line
    int h, m, s;
    
    Line tmpL;
    Time depT;
    Time depT_1;
    df.clear();
    db.clear();
    // fwd
    tmpL = net.getLine(l);
    for (auto stopf : tmpL.conns_fwd[0]) {
        depT = stopf.ti;
        if (stopf.ti == tmpL.conns_fwd[0][0].ti) depT_1 = depT;
        depT_1 = depT - depT_1;
        depT_1.gett(h, m, s);
        df.push_back(m);
        depT_1 = depT;
    }
    // odladeni
    /*
    cout << "line: " << l << endl;
    for (auto i : df) {
        cout << "df: " << i << endl;
    }
    */
    
    // bwd
    vector<Depart>::reverse_iterator stopb;
    int linlen = tmpL.stops.size();
    for (stopb = tmpL.conns_bwd[0].rbegin(); stopb != tmpL.conns_bwd[0].rend(); ++stopb) {
        depT = stopb->ti;
        if (stopb->ti == tmpL.conns_bwd[0][linlen-1].ti) depT_1 = depT;
        depT_1 = depT - depT_1;
        depT_1.gett(h, m, s);
        db.push_back(m);
        depT_1 = depT;
        
    }
    
    // odladeni
    /*
    cout << "line: " << l << endl;
    for (auto i : db) {
        cout << "db: " << i << endl;
    }
    cout << "--------";
    */

}

void get_rT (Network net, vector<int> & rtf, vector<int> & rtb, int l) {
    // rt - total time of the ride, l - line
    int h, m, s;
    Line tmpL;
    Time depT;
    Time depT_0;
    rtf.clear();
    rtb.clear();

    tmpL = net.getLine(l);
    // fwd - rtf
    for (auto stopf : tmpL.conns_fwd[0]) {
        depT = stopf.ti;
        if (stopf.ti == tmpL.conns_fwd[0][0].ti)  depT_0 = depT;
        depT = depT - depT_0;
        depT.gett(h, m, s);
        rtf.push_back(m);
    }
    // odladeni
    /*
    cout << "rTf for line: " << l << endl;
    for (auto i : rtf) {
        cout << "rtf: " << i << endl;
    }
    cout << "---------";
    */

    // bwd - rtb
    vector<Depart>::reverse_iterator stopb;
    int linlen = tmpL.stops.size();
    for (stopb = tmpL.conns_bwd[0].rbegin();  stopb != tmpL.conns_bwd[0].rend(); ++stopb) {
        depT = stopb->ti;
        if (stopb->ti == tmpL.conns_bwd[0][linlen-1].ti)  depT_0 = depT;
        depT = depT - depT_0;
        depT.gett(h, m, s);
        rtb.push_back(m);
    }
    // odladeni
    /*
    cout << "rTb for line: " << l << endl;
    for (auto i : rtb) {
        cout << "rtb: " << i << endl;
    }
    cout << "---------";
    */
}

void print_lineroute (Network net, vector<int> & df, vector<int> & db, vector<int> & rtf, vector<int> & rtb, int l)
{
    Line tmpL;
    tmpL = net.getLine(l);
    // the header
    cout << '+' << string(78, '-') << '+' << endl;
    cout << '|' << " Line: " << l << string(70, ' ') << '|' << endl;
    cout << "+---+----+" << string(29, '-') << "++---+----+" << string(29, '-') << '+' << endl;
    cout << "| d | rT | ==>" << string(25, ' ') << "|| d | rT | <==" << string(25, ' ') << '|' << endl;
    cout << "+---+----+" << string(29, '-') << "++---+----+" << string(29, '-') << '+' << endl;
    // the actual table XD
    for (size_t i = 0; i < tmpL.stops.size(); i++) {
        cout << "| "<<df[i]<<" | "<< setw(2)<< setfill('0')<<rtf[i]<<" | "<<tmpL.stops[i]<<string(28-tmpL.stops[i].size(), ' ')<<"|| ";
        cout << db[i]<<" | "<<setw(2)<< setfill('0')<<rtb[i]<<" | "<<tmpL.stops[(int)tmpL.stops.size()-1-i]<<string(28-tmpL.stops[(int)tmpL.stops.size()-1-i].size(), ' ')<<'|'<< endl;
    }
    cout << "+---+----+" << string(29, '-') << "++---+----+" << string(29, '-') << '+' << endl;
}

void load_drv_stats(Network net, DriverList dl,vector<Driver_stat> & drvv)
{
    Driver_stat drv_stat;
    Line tmpL;
    for (auto drv : dl) {
        // for evevry driver in the list
        drv_stat.name = drv.name;
        drv_stat.nC = 0;
        drv_stat.nSt = 0;
        for (int i = 0; i < net.nlines(); i++) {
            // search every line for the the conns this driver completed
            tmpL = net.getLine(i);
            for (auto connf : tmpL.conns_fwd) {
                if (connf.drv.name == drv.name) {
                    drv_stat.ln = i;
                    drv_stat.nC ++;
                    drv_stat.nSt += connf.size();
                }
            }
            for (auto connb : tmpL.conns_bwd) {
                if (connb.drv.name == drv.name) {
                    drv_stat.ln = i;
                    drv_stat.nC ++;
                    drv_stat.nSt += connb.size();
                }
            }
        }
        drvv.push_back(drv_stat);
    }
}

void print_drv_stats (vector<Driver_stat> & drvv) {   
    cout << '+'<<string(22, '-')<<"+---+----+-----+"<<endl;
    cout << "| Driver Name" << string(10,' ') << "| L | nC | nSt |" << endl;
    cout << '+'<<string(22, '-')<<"+---+----+-----+"<<endl;
    for (auto drv_st : drvv) cout << drv_st;
    cout << '+'<<string(22, '-')<<"+---+----+-----+"<<endl;
}

int main(int argc, char** argv) {

    // Load files:
    DriverList dl;
    dl.loadFromFile("driver_list.txt");

    Network net;
    net.loadFromFile("network.txt");

    // variables for argument decoding
    string flag_in;
    list<string> stop_in;

    vector<int> d_diff;
    vector<int> d_difb;
    vector<int> rt_diff;
    vector<int> rt_difb;

    vector<Driver_stat> drv_statv;

    if (argc > 1)
        flag_in = string(argv[1]);
    else
    {
        cerr << "Missing argument. Terminating." << endl;
        return 1;
    }

    if ((!flag_in.compare("--schedule")))
    {
        // input check - do not edit
        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                string temp = argv[i];
                replace(temp.begin(), temp.end(), '_', ' ');
                stop_in.push_back(temp);
            }
        }
        else
        {
            cerr << "Missing stop name. Terminating." << endl;
            return 2;
        }
        // end of input check

        /* here should start the code for printing timetables*/
        /* stop_in contains names of stops, for which the timetable is to be printed. */
        
        // This is an example, how it can be done using lambda-function
        // for_each(stop_in.begin(), stop_in.end(), [&] (string stop_name) {print_timetable(net,stop_name);} );
        // This is an example, how it can be done using for-loop 
        for( auto iter = stop_in.begin(); iter != stop_in.end(); iter++) print_timetable(net, *iter);  // *iter -- dereference iteratoru

        /* here should end the code for printing timetables*/
    }
    else if ((!flag_in.compare("--line-routing")))
    {

        /* here should start the code for printing line routes without stats*/
        for (int i = 0; i < net.nlines(); i++) {
            // todo fce na vypocteni d
            get_d(net, d_diff, d_difb, i);
            // todo fce na vypocteni rT
            get_rT(net, rt_diff, rt_difb, i);
            // todo fce na vytisteni tabulky
            print_lineroute(net, d_diff, d_difb, rt_diff, rt_difb, i);
        }


        /* here should end the code for printing line routes without stats*/
    }
    else if ((!flag_in.compare("--driver-stops")))
    {
        /* here should start the code for stops statistics for drivers */
        load_drv_stats(net, dl, drv_statv);
        // sort
        sort (drv_statv.begin(), drv_statv.end(), greater<Driver_stat>());
        // odladeni
        print_drv_stats(drv_statv);


        /* here should end the code for stops statistics for drivers */
    }
    else
    {
        cout << "Flag not recognized, terminating." << endl;
    }
    
    return 0;
}