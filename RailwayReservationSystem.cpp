#include <iostream>
#include <vector>
#include <list>

using namespace std;

class TrainReservationSystem {
public:
    TrainReservationSystem(int num_seats) {
        available_seats = num_seats;
        allocated_seats = vector<int>(num_seats, 0);
        needed_seats = vector<int>(num_seats, 0);
        waiting_list = list<int>();
    }

    bool reserve_seats(int passenger_id, int num_seats) {
        if (num_seats > available_seats) {
            waiting_list.push_back(passenger_id);
            return false;
        }

        for (int i = 0; i < num_seats; i++) {
            int available_seat = find_available_seat();
            allocated_seats[available_seat] = passenger_id;
            available_seats--;

            cout << "Seat " << available_seat << " has been assigned to passenger " << passenger_id << "." << endl;
        }

        return true;
    }

    void cancel_reservation(int passenger_id) {
        for (int i = 0; i < allocated_seats.size(); i++) {
            if (allocated_seats[i] == passenger_id) {
                allocated_seats[i] = 0;
                available_seats++;
            }
        }

        if (!waiting_list.empty()) {
            int passenger_id_at_top_of_waiting_list = waiting_list.front();
            waiting_list.pop_front();

            bool request_success = reserve_seats(passenger_id_at_top_of_waiting_list, 1);
            if (request_success) {
                cout << "Passenger " << passenger_id_at_top_of_waiting_list << " has been assigned the seat canceled by passenger " << passenger_id << "." << endl;
            } else {
                cout << "Failed to reserve a seat for passenger " << passenger_id_at_top_of_waiting_list << "." << endl;
            }
        }
    }

    bool isRequestSuccessful(int passenger_id) {
        for (int i = 0; i < allocated_seats.size(); i++) {
            if (allocated_seats[i] == passenger_id) {
                return true;
            }
        }

        return false;
    }

private:
    int available_seats;
    vector<int> allocated_seats;
    vector<int> needed_seats;
    list<int> waiting_list;

    int find_available_seat() {
        for (int i = 0; i < allocated_seats.size(); i++) {
            if (allocated_seats[i] == 0) {
                return i;
            }
        }
        return -1;
    }
};

int main() {
    int num_seats;
    cout << "Enter the number of seats: ";
    cin >> num_seats;

    TrainReservationSystem train_reservation_system(num_seats);

    int num_passengers;
    cout << "Enter the number of passengers: ";
    cin >> num_passengers;

    for (int passenger_id = 1; passenger_id <= num_passengers; passenger_id++) {
        int num_seats_required;
        cout << "Enter the number of seats required for passenger " << passenger_id << ": ";
        cin >> num_seats_required;

        bool request_success = train_reservation_system.reserve_seats(passenger_id, num_seats_required);
        if (!request_success) {
            cout << "Request failed for passenger " << passenger_id << "." << endl;
        }
    }

    while (true) {
        int passenger_id_to_cancel;
        cout << "Enter the passenger ID to cancel reservation (or enter -1 to exit): ";
        cin >> passenger_id_to_cancel;

        if (passenger_id_to_cancel == -1) {
            break;
        }

        train_reservation_system.cancel_reservation(passenger_id_to_cancel);
    }

    bool allRequestsSuccessful = true;
    for (int passenger_id = 1; passenger_id <= num_passengers; passenger_id++) {
        if (!train_reservation_system.isRequestSuccessful(passenger_id)) {
            allRequestsSuccessful = false;
            break;
        }
    }

    if (allRequestsSuccessful) {
        cout << "All passengers' requests are successfully accommodated, and the reservations are made." << endl;
    } else {
        cout << "Not all passengers' requests could be accommodated." << endl;
    }

    return 0;
}
