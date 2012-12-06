#pragma once

class ProFy {

private:
							//! Class constructor 
							ProFy();

public:
							//! Class destructor
							~ProFy();

							//! Get access to the singleton
	static ProFy			*Instance();

							//! Start an openGL timer
	void					StartGLTimer(
								int &id								//!< An id to get the result of the timer
							);

							//! End an openGL timer
							//! \Result True on success
	const bool				EndGLTimer(
								int id								//!< The timer ID of the result you wish to obtain
							)					

};
