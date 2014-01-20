#include "NoteList.h"

namespace Music {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createMajorScale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NoteList createMajorScale( const Note& root )
{
   int scaleDef[7] = { 0, 2, 2, 1, 2, 2, 2 };
   std::vector< int > scaleDefVec( scaleDef, scaleDef + 7 );
   const NoteList& result = createGenericScale( root, scaleDefVec );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createMinorScale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NoteList createMinorScale( const Note& root )
{
   int scaleDef[7] = { 0, 2, 1, 2, 2, 1, 2 };
   std::vector< int > scaleDefVec( scaleDef, scaleDef + 7 );
   const NoteList& result = createGenericScale( root, scaleDefVec );
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createGenericScale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NoteList createGenericScale( const Note& root, const std::vector<int>& scaleDef )
{
   NoteList result;
   Note note( root );
   for ( size_t i = 0; i < scaleDef.size(); ++i )
   {
      note.modifyPitch( scaleDef[i] );
      result.push_back(note);
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createHugeDiatonicScale
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NoteList createHugeDiatonicScale()
{
   Music::Note note( Note::C, 2 );
   std::vector< int > scaleDef;
   for ( size_t i = 0; i < 12 * 5; ++i )
   {
      scaleDef.push_back( 1 );
   }
   return Music::createGenericScale( note, scaleDef );
}

} /// namespace Music
