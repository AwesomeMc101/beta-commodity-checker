#include "math_lib.hpp"
#include "classes.hpp"
#include "error_scanner.hpp"
//typedef enum {
//	WTI,
//	BRENT,
//	NATURALGAS,
//	
//	ALUMINUM,
//	COPPER,
//
//	CORN,
//	WHEAT,
//	COFFEE,
//	COTTON,
//	SUGAR
//
//
//} CORRELATED_TYPES;


typedef enum {
	/* OIL */
	OIL_MAC,
	OIL_DC,
	OIL_QPC,


} CORRELATED_EQUATION; //MAC for oil would be an example of this

#define COR_GLOBAL_OIL 300
#define COR_GLOBAL_METAL 400
#define COR_GLOBAL_CROP 500

class Correlated_Stock { //no reason to waste time making functions for everything here, just make it public
public:
	Stock* stk_ptr;
	std::vector<COMMODITY_TYPE> correlations;
	std::vector<CORRELATED_EQUATION> correlations_equations;

	int cor_global_type; //COR_GLOBAL_OIL, COR_GLOBAL_METAL, COR_GLOBAL_CROP

};

namespace Decide_Correlations
{

	bool check(float);

	void set_global_type(Correlated_Stock*);

	void check_oil(Correlated_Stock*);
	void check_metals(Correlated_Stock*, Stock*);
	void check_crops(Correlated_Stock*, Stock*);

	Correlated_Stock* form_correlated_stock(Stock*);
	

}
