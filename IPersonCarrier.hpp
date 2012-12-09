/*
 * Copyright (c) 2012, Joseph Max DeLiso
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 */

#ifndef _I_PERSON_CARRIER_H
#define _I_PERSON_CARRIER_H

#include <unordered_set>
#include <unordered_map>

namespace elevatorSim {
class Person;

class IPersonCarrier {

   static std::unordered_map<Person*, IPersonCarrier*>* containerCache;

   static std::unordered_map<Person*, IPersonCarrier*>* acquireContainerCache();
   inline static void invalidateCCEntry( Person * const cp );
   inline static void updateCCEntry( Person * const cp, IPersonCarrier* icp );

protected:

   std::unordered_set<Person*> people;

public:

   inline static Person* checkContainerCache( Person * const cp );
   static void cleanContainerCache();

   enum PERSON_CARRIER_TYPE {
      FLOOR_CARRIER,
      ELEVATOR_CARRIER
   };

   bool containsPerson( const Person* const & p ) const {
      return people.find( const_cast<Person* const &>( p ) ) != people.end();
   }

   void addPerson( Person * const p ) {
      std::pair<std::unordered_set<Person*>::iterator, bool> ret =
               people.insert(p);

      /* ensure that the add succeeded */
      assert(ret.second);
   }

   bool removePerson( Person* p ) {
      return ( people.erase(p) > 0 );
   }

   int numPeopleContained() const {
      return people.size();
   }

   PyObject* peopleToTuple() const {
      /* TODO */
      return NULL;
   }

   void decrefPeopleTuple(PyObject* peopleTuple){
      (void) peopleTuple;

      /* TODO */
   }

   virtual enum PERSON_CARRIER_TYPE getCarrierType() const = 0;

   virtual ~IPersonCarrier() = 0;
};

} /* namespace elevatorSim */

#endif /* _I_PERSON_CARRIER_H */
