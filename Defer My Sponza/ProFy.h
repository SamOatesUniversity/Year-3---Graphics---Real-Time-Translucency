#pragma once

#include <glm/glm.hpp>
#include "framework/tgl.h"

#include <vector>

class ProFy {

private:
							//! Class constructor 
							ProFy();

	struct Timer {
		GLuint id;
		bool waitingForResult;
		bool hasEnded;
	};

	std::vector<Timer>		m_timers;

private:

	int						GetTimer(
								int id
							);

public:
							//! Class destructor
							~ProFy();

							//! Get access to the singleton
	static ProFy			*GetInstance();

							//! Create an openGL timer
	void					CreateGLTimer(
								GLuint &id							//!< An id to get the result of the timer
							);

							//! Start an openGL timer
	void					StartGLTimer(
								GLuint &id							//!< An id to get the result of the timer
							);

							//! End an openGL timer
							//! \Result Time between start and stop
	const GLuint64			EndGLTimer(
								GLuint id							//!< The timer ID of the result you wish to obtain
							);			

};
