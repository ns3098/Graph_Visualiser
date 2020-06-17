

#ifndef CHANGES_API_H
#define CHANGES_API_H

#define CHANGES_VARS int ChangesDeepFlag;
#define CHANGES_INIT ChangesDeepFlag = 0;

#define CHANGES_BEGIN \
	if(!ChangesDeepFlag){emit ABOUT_TO_CHANGE_DEF();}\
	++ChangesDeepFlag;

#define CHANGES_END \
	--ChangesDeepFlag;\
	if(!ChangesDeepFlag){emit CHANGED_DEF();}

#endif // CHANGES_API_H
