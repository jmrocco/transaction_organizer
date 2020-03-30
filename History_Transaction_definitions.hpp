//Juliette Rocco
//November 29th 2019
//Mo Money: Adjusted Cost Base Project
#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>


#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"


////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////


//The constructor for the transaction class
//Takes in the following variables:
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
		unsigned int month_date,  unsigned year_date,
		bool buy_sell_trans,  unsigned int number_shares,
		double trans_amount )
{
	//initializes the private variables from transaction with the arguments
	 this->symbol = ticker_symbol;
	 this->day = day_date;
	 this->month = month_date;
	 this->year = year_date;
	 this->shares = number_shares;
	 this->amount = trans_amount;
	 this->trans_id = assigned_trans_id;

	 //Since buy_sell_trans is a bool and trans type is a string,
	 //if it is true then it is "Buy" and "Sell" if false
	 if(buy_sell_trans){
		 this->trans_type = "Buy";
	 }
	 else{
		 this->trans_type = "Sell";
	 }

	 //increments the assigned trans id so all transactions
	 //have a different id
	 ++assigned_trans_id;
	 //by default the next pointer of each transaction will be to the nullptr
	 //because there may not be another transaction
	 p_next = nullptr;

	 //initialize the following calculations to 0
	 //to ensure everything is 0 at the beginning
	 this->acb = 0;
	 this->acb_per_share = 0;
	 this-> share_balance = 0;
	 this->cgl = 0;
}

//Destructor for the transaction class: It is empty
//because we don't have anything we need to destruct
Transaction::~Transaction()
{
}

//Function for the transaction class for the '<' operator
//returns true if the transaction is less than the other one we
//are comparing with
bool Transaction::operator<( Transaction const &other )	{
	//a default return value for if none of the cases are true
	bool return_value{false};

	//if the year is less return true
	if (this->year < other.year){
		return_value = true;
	}
	else{
		//if the years are equal then we need to check the month
		if(this->year == other.year){

			//if the month of the first one is less, return true
			if(this->month < other.month){
				return_value = true;
			}
			else{

				//if the months are equal we need to check the days
				if (this->month == other.month){

					//if the day of the first one is less return true
					if (this->day < other.day){
						return_value = true;
					}
				}
			}
		}
	}
//returns the return value
return return_value;
}

// Member functions of the transaction class to get values
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }


//Member functions to set values from the transaction class
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

//Print function from the transaction class
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////


// Constructor for the history class
History::History(){
	//sets the head of our node initially to
	//nullptr because we don't have any nodes
	//in the list
	p_head = nullptr;
}


// Destructor for the history class
History::~History(){
	//this destructor deletes all the nodes in the linked list
	//we have p_traverse, which walks through the linked list
	Transaction *p_traverse{p_head};

	while(p_traverse != nullptr){
		//moves the traverse node one forward but does not change the head
		//deletes the node at the head and then sets the new head to p_traverse
		//we have to move traverse forward before deleting because we
		//lose the node if we don't move it
		p_traverse = p_traverse->get_next();
		delete p_head;
		p_head = p_traverse;
	}

}

//This functions reads the transaction file and creates the linked list
void History::read_history(){
	//opens the file and gets the first entry
	ece150::open_file();
	bool end_of_file{ece150::next_trans_entry()};

	//until its the end of the file...
	while(end_of_file){

		//takes the following values from the file
		std::string ticker_symbol = ece150::get_trans_symbol();
		unsigned int day_date = ece150::get_trans_day();
		unsigned int month_date = ece150::get_trans_month();
		unsigned int year_date = ece150::get_trans_year();
		bool buy_sell_trans = ece150::get_trans_type();
		unsigned int number_shares = ece150::get_trans_shares();
		double trans_amount = ece150::get_trans_amount();

		//create a new instance of the class and initializes with the values above
		Transaction *p_new_trans = new Transaction{ticker_symbol,day_date,month_date,year_date,buy_sell_trans,number_shares,trans_amount};

		//inserts the transaction into the history linked list
		insert(p_new_trans);

		//gets the next entry
		end_of_file = ece150::next_trans_entry();
	}
	//closes the file when it is finished reading
	ece150::close_file();
}


//inserts a transaction into the linked list, uses push from back method
void History::insert(Transaction *p_new_trans){
	//we have p_traverse, which walks through the linked list
	Transaction *p_traverse{p_head};

	//if there are no entries in the linked list which is represented
	//by p_head pointing to nullptr, set the head to the new transaction
	if (p_head == nullptr){
		p_head = p_new_trans;

	//if the list is not empty
	} else {
		//we walk the traverse node to the end of the list
		while(p_traverse->get_next() != nullptr){
			p_traverse = p_traverse->get_next();
		}
		//set traverse's next node as the new transaction
		//this is the push from back method
		p_traverse->set_next(p_new_trans);

	}

}

void History::sort_by_date(){
	//p_sorted is the sorted array, unsorted is the original history, and p_unlinked
	//temporary stores a transaction from p_unsorted
	Transaction *p_sorted{nullptr};
	Transaction *p_unsorted{p_head};
	Transaction *p_unlinked{nullptr};

	//we loop through the unsorted history until
	//we reach the end
	while(p_unsorted != nullptr){

		//we set the unlinked to the first element in p_unsorted
		//and then move unsorted to the next element
		p_unlinked = p_unsorted;
		p_unsorted = p_unsorted->get_next();

		//if the sorted history has no entries
		//set the first entry to the unlinked item and point
		//unlinked to nullptr
		if (p_sorted == nullptr){
			p_sorted = p_unlinked;
			p_sorted->set_next(nullptr);
		}
		//if the history is not empty there are 3 cases
		else{

			//first case is if the unlinked item is less than the first
			//item in the history of sorted
			if (*p_unlinked < *p_sorted){
				//set unlinks next item to sorted
				//reset sorted to the beginning which is now "unlinked"
				p_unlinked->set_next(p_sorted);
				p_sorted = p_unlinked;
			}

			//second case is if there is only one element in sorted
			else if (p_sorted->get_next() == nullptr){
				//sets sorted next element to unlinked
				//unlinked is now the last element and points to nullptr
				p_sorted->set_next(p_unlinked);
				p_unlinked->set_next(nullptr);
			}

			//3rd case is if we have to insert in between transactions
			else{
				//create a temporary transaction to walk through sorted
				//we create a temp because we don't want to lose the head
				//of p_sorted
				Transaction *p_temp(p_sorted);
				//while we don't reach the end of the history and the unlinked object is greater than the next entry
				//we advance temp to the next item in history
				//we use p_temp->get_next() as a condition because we can't go backwards if we go too far
				while((p_temp->get_next() != nullptr) && (*(p_temp->get_next()) < *p_unlinked)){
					p_temp = p_temp->get_next();
				}

				//we have found where it must be inserted, set p_unlinked to the next
				//transaction in temp and set temp to p_unlinked
				p_unlinked->set_next(p_temp->get_next());
				p_temp->set_next(p_unlinked);
			}

		}
	}
	//once we finish sorting set the head to the new sorted history
	p_head= p_sorted;
}
//Updates the ACB and CGL values.
void History::update_acb_cgl(){
	//we have p_traverse, which walks through the linked list
	Transaction *p_traverse{p_head};

	//counters for the ACB and CGL values, we
	//accumulate values for these variables
	unsigned int total_shares{};
	double acb{},acb_per_share{}, cgl{};

	//we walk our traverse node to the end of the linked list
	while(p_traverse != nullptr){

		//if the node is a buy node
		if (p_traverse->get_trans_type()){
			//total shares is calculated by adding the number of shares
			//bought to the counter, then it sets this value for the node
			total_shares += p_traverse->get_shares();
			p_traverse->set_share_balance(total_shares);
			//acb is calculate by accumulating the total amount paid for the
			//shares, then it sets this value for the node
			acb += p_traverse->get_amount();
			p_traverse->set_acb(acb);
			//the acb per share is calculated by dividing the acb
			//by the total number of shares, then the value is set for the node
			acb_per_share = p_traverse->get_acb() / p_traverse->get_share_balance();
			p_traverse->set_acb_per_share(acb_per_share);
		}
		//if the node is a sell node
		else{
			//total shares is calculated by subtracting the number of shares
			//from the counter, then it sets this value for the node
			total_shares -= p_traverse->get_shares();
			p_traverse->set_share_balance(total_shares);
			//acb is calculated by subtracting the following:
			//the number of shares sold in the transaction multiplied
			//by the acb from the previous transaction which is the acb counter
			//we have been accumulating, the value is then set for the node
			acb -= (p_traverse->get_shares() * acb_per_share);
			p_traverse->set_acb(acb);
			//acb per share is calculated by dividing the acb
			//by the total number of shares, then the value is set for the node
			acb_per_share = p_traverse->get_acb() / p_traverse->get_share_balance();
			p_traverse->set_acb_per_share(acb_per_share);
			//the cgl is calculated by subtracting from the amount paid the number of shares
			//multiplied by the number of shares, this value is then set for the node
			cgl = p_traverse->get_amount() - (p_traverse->get_shares() * p_traverse->get_acb_per_share());
			p_traverse->set_cgl(cgl);

		}
			//advance p_traverse to the next node
			p_traverse = p_traverse->get_next();
	}


}

//Capital gain and loses for the year specified
double History::compute_cgl(unsigned int year){
	//we have p_traverse, which walks through the linked list
	Transaction *p_traverse{p_head};
	//create a counter for the cgl
	double cgl{0};

	//walk traverse through the linked list
	while(p_traverse != nullptr){

		//we only have capital gains and loses on sell
		//transactions, do not want to look at buy transaction
		if (p_traverse->get_trans_type() == false){
			//if the year of our transaction matches the desired year
			//add the cgl for our transaction to our counter
			if (p_traverse->get_year() == year){
					cgl += p_traverse->get_cgl();
			}
		}
		//advance our traverse node to the next node
		p_traverse = p_traverse->get_next();
	}
	//return the cgl calculated for the given year
	return cgl;
}

//When called prints the linked list entries
void History::print(){
	//The beginning header
	std::cout << "============ BEGIN TRANSACTION HISTORY ============"<<std::endl;

	//we have p_traverse, which walks through the linked list
	Transaction *p_traverse{p_head};

	//traverse walks through the linked list
	while(p_traverse != nullptr){
		//each transaction prints and then advances
		//to the next pointer
		p_traverse->print();
		p_traverse = p_traverse->get_next();
	}
	//prints the ending header
	std::cout << "============ END TRANSACTION HISTORY ============"<<std::endl;
}

// get_p_head(): Full access to the linked list.
Transaction *History::get_p_head() { return p_head; }


#endif

