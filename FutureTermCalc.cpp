#include <iostream>
#include <cmath>
#include <vector>


//No AI, 100% Human Made and De-Bugged
//Made With ONLY PowerShell anb NotePad
//Credits GitHub: RobertNikolasDaniel


//==========================================================================================================


//Market Basis Function
double marketbasis( double far, double near ) {
	return ( far - near ) / near;
}


//==========================================================================================================


//Funding Spread Function
double fundingspread( double yield, double sofr, double days ) {
	return ( yield * ( days / 365 ) ) - ( sofr * ( days / 365 ) );
}


//==========================================================================================================


//Funding Spread Eulers Fair Value DF Function
double fairvalue( double carry ) {
	return std::exp( -carry );
}


//==========================================================================================================


//MAIN
int main() {

	//Welcome Text
	std::cout << "\n---Futures Term Structure Market vs Fair Value---" << std::endl;
	std::cout << "---NO AI USED, Only PowerShell and NotePad---" << std::endl;

	//define variables
	double spot, near, far, yield, monthsnear, daysnear, monthsfar, daysfar;


	//==========================================================================================================


	//---INPUTS AND SOFR VECTORS---
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n---Type In Inputs Below---\n" << std::endl;
	std::cout << "\n";
	std::cout << "\n";

	//spot underlying
	std::cout << "\nSpot Underlying Price: ";
	std::cin >> spot;

	//near future
	std::cout << "\n(near)Future Price: ";
	std::cin >> near;

	//far future
	std::cout << "\n(far)Future Price: ";
	std::cin >> far;

	//underlying yield or dividend
	std::cout << "\nAsset Yield or Dividend in Basis Points: ";
	std::cin >> yield;

	//months to delivery NEAR
	std::cout << "\nMonths to delivery (near)Future: ";
	std::cin >> monthsnear;


	//---NEAR FUTURE VECTOR MATH---

	//store vector for near sofr
	double nearcount;
	std::vector<double> nearstore = { };
		for ( nearcount = 0; nearcount < monthsnear; nearcount++ ) {
			double x;
			std::cout << "\nType In Annualized SOFR, per CME SR1, for month " << nearcount + 1 << " in Basis Points: ";
			std::cin >> x;
			nearstore.push_back(x);
			}

	//find average sofr in vector nearstore
	double nearavg = 0;
	for ( double x : nearstore ) {
		nearavg += x;
		}
	nearavg = nearavg / nearstore.size();
	std::cout << "\n---Average SOFR for (near)Future Delivery is: " << nearavg << std::endl;
		


	//near future days to delivery
	std::cout << "\nDays to Delivery (near)Future: ";
	std::cin >> daysnear;
	

	//months to delivery FAR
	std::cout << "\nMonths to delivery (far)Future: ";
	std::cin >> monthsfar;


	//---FAR FUTURE VECTOR MATH---

	//store vector for far sofr
	double farcount;
	std::vector<double> farstore = { };
		for ( farcount = 0; farcount < monthsfar; farcount++ ) {
			double y;
			std::cout << "\nType In Annualized SOFR, per CME SR1, for month " << farcount + 1 << " in Basis Points: ";
			std::cin >> y;
			farstore.push_back(y);
			}

	//find average sofr in farstore
	double faravg = 0;
	for ( double y : farstore ) {
		faravg += y;
		}
	faravg = faravg / farstore.size();
	std::cout << "\n---Average SOFR for (far)Future Delivery is: " << faravg << std::endl;


	//far future days to delivery
	std::cout << "\nDays to Delivery (far)Future: ";
	std::cin >> daysfar;


	//==========================================================================================================


	//---NEAR FUTURE VS SPOT OUTPUTS---
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n---(near)FUTURE vs SPOT OUTPUTS---";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";


	//funding spread TIME ADJUSTED means the actual carry (non annualized)
	std::cout << "\n(near)Future Funding Spread (yield - sofr) Time Adjusted: " << fundingspread( yield, nearavg, daysnear ) << "bps" << std::endl;


	//converting basis points to decimal
	double fundingspreadD = fundingspread( yield, nearavg, daysnear ) / 10000;

	//finding fair value of near future
	double nearfv = spot * fairvalue( fundingspreadD );
	

	//calculating fair value (near)Future
	std::cout << "\n(near)Future Fair Value from Carry: " << nearfv << std::endl;

	//market future vs fair value point spread
	if ( nearfv < near ) {
		std::cout << "\n(near)Future Market Price of " << near << " is " << near - nearfv << " points above the Fair Value" << std::endl;
	} else if ( nearfv > near ) {
		std::cout << "\n(near)Future Market Price of " << near << " is " << near - nearfv << " points below the Fair Value" << std::endl;
	} else {
		std::cout << "\n(near)Future Market Price trades exactly at the Fair Value" << std::endl;
	}

	//market traded basis
	double marketbasisdecimal = marketbasis( near, spot );
	double marketbasisbps = marketbasisdecimal * 10000;

	//market basis
	std::cout << "\nThe Market Traded Basis of (near)Future to Spot: " << marketbasisbps << "bps" << std::endl;

	//term richness and cheapness
	if ( marketbasisbps > fundingspread( yield, nearavg, daysnear ) ) {
		std::cout << "\n(near)Future Trades at Term Richness by: " << std::abs(marketbasisbps) - std::abs( fundingspread( yield, nearavg, daysnear ) ) << "bps" << std::endl;
	} else if ( marketbasisbps < fundingspread( yield, nearavg, daysnear ) ) {
		std::cout << "\n(near)Future Trades at Term Cheapness by: " << std::abs(marketbasisbps) - std::abs( fundingspread( yield, nearavg, daysnear ) ) << "bps" << std::endl;
	} else {
		std::cout << "\n(near)Future has zero Term Distortions because it trades at Fair Value" << std::endl;
	}


	//==========================================================================================================


	//---FAR FUTURE VS SPOT OUTPUTS---
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n---(far)FUTURE vs SPOT OUTPUTS---";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";

	//funding spread
	std::cout << "\n(far)Future Funding Spread (yield - sofr) Time Adjusted: " << fundingspread( yield, faravg, daysfar ) << "bps" << std::endl;

	//converting basis points to decimal
	double fundingspreadDD = fundingspread( yield, faravg, daysfar ) / 10000;

	//finding fair value of far future
	double farfv = spot * fairvalue( fundingspreadDD );

	//calculating fair value (far)Future
	std::cout << "\n(far)Future Fair Value from Carry: " << farfv << std::endl;

	// market future vs fair value point spread
	if ( farfv < near ) {
		std::cout << "\n(far)Future Market Price of " << far << " is " << far - farfv << " points above the Fair Value" << std::endl;
	} else if ( farfv > far ) {
		std::cout << "\n(far)Future Market Price of " << far << " is " << far - farfv << " points below the Fair Value" << std::endl;
	} else {
		std::cout << "\n(far)Future Market Price trades exactly at the Fair Value" << std::endl;
	}

	//market traded basis
	double marketbasisddecimal = marketbasis( far, spot );
	double marketbasisbbps = marketbasisddecimal * 10000;

	//market basis
	std::cout << "\nThe Market Traded Basis of (far)Future to Spot: " << marketbasisbbps << "bps" << std::endl;

	//term richness and cheapness
	if ( marketbasisbbps > fundingspread( yield, faravg, daysfar ) ) {
		std::cout << "\n(far)Future Trades at Term Richness by: " << std::abs(marketbasisbbps) - std::abs( fundingspread( yield, faravg, daysfar ) ) << "bps" << std::endl;
	} else if ( marketbasisbps < fundingspread( yield, nearavg, daysnear ) ) {
		std::cout << "\n(far)Future Trades at Term Cheapness by: " << std::abs(marketbasisbbps) - std::abs( fundingspread( yield, faravg, daysfar ) ) << "bps" << std::endl;
	} else {
		std::cout << "\n(far)Future has zero Term Distortions because it trades at Fair Value" << std::endl;
	}


	//==========================================================================================================


	//---(near)FUTURE vs (far)FUTURE Calendar Spread Outputs---
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n(far)FUTURE vs (near)FUTURE CALENDAR SPREAD OUTPUTS";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";

	//Futures Carry Spread
	std::cout << "\n(far)Future minus (near)Future Carry Spread is: " << fundingspread( yield, faravg, daysfar ) - fundingspread( yield, nearavg, daysnear) << "bps" << std::endl;

	//Fair Value Calendar Spread from Carry Spread
	std::cout << "\n(far)Future minus (near)Future Fair Value Calendar Spread in Points is: " << farfv - nearfv << std::endl;

	//Market Traded Calendar Spread
	std::cout << "\n(far)Future minus (near)Future Market Traded Calendar Spread is: " << marketbasisbbps - marketbasisbps << "bps" << std::endl;

	//Market Traded Calendar Spread Points
	std::cout << "\n(far)Future minus (near)Future Market Traded Calendar Spread in Points is: " << far - near << std::endl;
	
	//Calendar Spread Differential
	if ( (far - near) > (farfv - nearfv) ) {
		std::cout << "\nThe Market Calendar Spread is " << (far - near) - (farfv - nearfv) << " Points above the Fair Value" << std::endl;
	} else if ( (far - near) < (farfv - nearfv) ) {
		std::cout << "\nThe Market Calendar Spread is " << (far - near) - (farfv - nearfv ) << " Points below the Fair Value" << std::endl;
	} else {
		std::cout << "\nThe Market Calendar Spread is Trading at the Fair Value" << std::endl;
	}
		

	std::cout << "\n";
	std::cout << "\n";


	//RETURN 0
	return 0;
}
