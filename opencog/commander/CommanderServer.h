/*
 * CommanderServer.h
 *
 * Copyright (C) 2017 MindCloud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_COMMANDER_SCM_H
#define _OPENCOG_COMMANDER_SCM_H

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/truthvalue/TruthValue.h>
#include <pthread.h>

namespace opencog
{

class CommanderServer
{
public:
  CommanderServer();
  int start_server(void);
  int stop_server(void);
private:
  	static void * server_thread(void *); 
	  pthread_t worker;
  /**
   * The main init function for the CommanderServer object.
   */
  void init(void);
};

} // namespace opencog

extern "C" {
void opencog_commander_init(void);
};

#endif  // _OPENCOG_OPENPSI_SCM_H
