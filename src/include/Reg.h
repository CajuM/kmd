/*
 * Reg.h
 *
 *  Created on: Jan 30, 2016
 *      Author: mihai
 */

#ifndef SRC_REG_H_
#define SRC_REG_H_

#include <int.h>

namespace kmd {

class Reg {
public:
	Reg(const char * name);
	virtual ~Reg();

	template<typename T> void set(T val);
	template<typename T> T get();

private:
	const char * name;
};

} /* namespace kmd */

#endif /* SRC_REG_H_ */
