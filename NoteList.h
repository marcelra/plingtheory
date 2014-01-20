#ifndef NOTELIST_H
#define NOTELIST_H

#include "Note.h"

#include <vector>

namespace Music
{
   typedef std::vector<Note> NoteList;

   /**
    * Create a major scale 1, 2, 4, 5, 7, 9, 11
    */
   NoteList createMajorScale( const Note& root );
   /**
    * Create a minor scale 1, 2, 3, 5, 7, 6, 10
    */
   NoteList createMinorScale( const Note& root );

   /**
    * Create a generic scale. @param scaleDef contains number of half steps w.r.t. previous note
    * E.g. a major scale is: 0, 2, 2, 1, 2, 2, 2
    */
   NoteList createGenericScale( const Note& root, const std::vector<int>& scaleDef );

   /**
    * Create a `scale' from all the notes between C0 to C10
    */
   NoteList createHugeDiatonicScale();

} /// namespace Music

#endif // NOTELIST_H
