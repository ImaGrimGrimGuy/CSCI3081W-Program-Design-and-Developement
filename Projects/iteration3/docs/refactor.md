# Refactored Code:

## Variable Renaming:

### Replace "templight" with "lightHolder"

See lines 71-74 in arena.cc

Similar to "temp Guy" this was a sort of joke name to be renamed upon release. It was <br /> 
intended to be a placeholder name for something more descriptive. "lightHolder" is <br /> 
as descriptive a term for a variable that holds a pointer to a "light" object for <br /> 
later use as the developers could fathom for now.


### Replace "tempguy" with "botPlaceholder"

See lines 78-81 in arena.cc

Originally, "tempguy" was kind of a joke name for a variable that was supposed to be <br /> 
used for passing robots into the arena_mobile_entities_ vector in arena. Since its only <br /> 
purpose was to hold a copy of a pointer so that that pointer could be placed in multiplie <br /> 
vectors, the name "botPlaceHolder" seemed fitting.

## Extracted Methods:

### Extracted Method "MobileEntityUpdate" in Arena

See lines 146-164 in arena.cc

Originally, the method "UpdateEntitiesTimestep()" method was a massive doubly-nested <br /> 
for-loop that controlled bassically everything in the simulation. To make this easier <br /> 
to read and understand, the mechanics of the outer loop were placed into the <br /> 
"MobileEntityUpdate" function. It is more clear because it encapsulates the exact <br /> 
process of updating one mobile entity in one method.

### Extracted Method "CheckOtherEnts" in Arena

See lines 166-193 in arena.cc

Inside of the "MobileEntityUpdate" function, there is another function devoted to <br /> 
the process of checking how the other ArenaEntities affect the current mobile entity <br /> 
being updated. Originally this method was also in "UpdateEntitiesTimestep", but was <br /> 
moved due to it being a complicated process that was better understood when encapsulated <br /> 
in its own method.
