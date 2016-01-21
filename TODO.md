#Bugs 

- done ->  mixed the axis x and y at configuration mode creating of area size
- how to react on deltaMass > tempMass in calcAirDeltaMoleculesCount()
- programm does not react on errors at load and saveAreaStructureTemplate
- not yet programmed all possible ways in main

#Documentation 

- JavacDoc-Header of
	- Cubes Class

#Code-Refactoring
- move matrix handling code to template class matrix
- set some getters and setters to private if calc-Functions have to be used from outside
- overload scalar_multiplication for vector3
- insert getter and setter for every attribute and set them private if not needed
- proof for direct manipulation without getter and setter
- in Main : replace things like "Enter '+'" by "Enter [+]"
- set const functions to const
