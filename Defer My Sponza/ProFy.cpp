#include "ProFy.h"


ProFy::ProFy()
{

}

ProFy::~ProFy()
{

}

ProFy *ProFy::GetInstance()
{
	static ProFy instance;
	return &instance;
}

void ProFy::CreateGLTimer( 
		GLuint &id											//!< An id to get the result of the timer
	)
{
	glGenQueries(1, &id);

	Timer newTimer;
	newTimer.id = id;
	newTimer.waitingForResult = false;
	newTimer.hasEnded = false;

	m_timers.push_back(newTimer);
}

void ProFy::StartGLTimer( 
		GLuint &id											//< An id to get the result of the timer
	)
{
	Timer &timer = m_timers[GetTimer(id)];
	if (!timer.waitingForResult)
	{
		glBeginQuery(GL_TIME_ELAPSED, id);
		timer.waitingForResult = true;
		timer.hasEnded = false;
	}
}

const GLuint64 ProFy::EndGLTimer( 
		GLuint id											//!< The timer ID of the result you wish to obtain
	)
{
	Timer &timer = m_timers[GetTimer(id)];

	if (!timer.hasEnded)
	{
		glEndQuery(GL_TIME_ELAPSED);
		timer.hasEnded = true;
	}
	
	GLint hasResult = 0;
	glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &hasResult);
	if (!hasResult)
		return NULL;

	timer.waitingForResult = false;

	GLuint64 time = 0;
	glGetQueryObjectui64v(id, GL_QUERY_RESULT, &time);
	return time;
}

int ProFy::GetTimer( 
		int id 
	)
{
	int index = 0;
	for (Timer timer : m_timers)
	{
		if (timer.id == id)
			return index;

		index++;
	}

	return -1;
}
