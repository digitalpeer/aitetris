/*************************************************************************/
/*									 */
/*	Source code for use with See5/C5.0 Release 1.17			 */
/*	-----------------------------------------------			 */
/*		      Copyright RuleQuest Research 2002			 */
/*									 */
/*	This code is provided "as is" without warranty of any kind,	 */
/*	either express or implied.  All use is at your own risk.	 */
/*									 */
/*************************************************************************/


/*************************************************************************/
/*									 */
/*		General data for See5/C5.0				 */
/*									 */
/*************************************************************************/


	Attribute	ClassAtt=0,	/* attribute to use as class */
			LabelAtt=0;	/* attribute used as case label */
	int		MaxAtt=0,	/* max att number */
			MaxClass=0,	/* max class number */
			ErrMsgs=0,      /* errors found */
			AttExIn=0,	/* attribute exclusions/inclusions */
			TSBase=0,	/* base day for time stamps */
			LineNo=0,	/* input line number */
			Delimiter;	/* character at end of name */
	DiscrValue	*MaxAttVal=0;	/* number of values for each att */
	char		*SpecialStatus=0;	/* special att treatment */
	Definition	*AttDef=0;	/* attribute definitions */
	String		*ClassName=0,	/* class names */
		  	*AttName=0,	/* att names */
		  	**AttValName=0;	/* att value names */
	ClassNo		*TrialPred=0;	/* predictions for each boost trial */
	float		Confidence,	/* set by classify() */
			*Vote=0,	/* total votes for classes */
			**MCost=0;	/* misclass cost [pred][real] */
	Tree		*Pruned=0;	/* decision trees */
	CRuleSet	*RuleSet=0;	/* rulesets */
	FILE		*TRf=0;		/* file pointer for tree and rule i/o */
	char		*FileStem="?",	/* file stem */
			Fn[500];	/* file name with extension */
	Boolean		*SomeMiss=Nil,	/* att has missing values */
			*SomeNA=Nil;	/* att has N/A values */


/*************************************************************************/
/*									 */
/*		Global parameters for See5/C5.0				 */
/*									 */
/*************************************************************************/


	int		TRIALS=1;	/* number of classifiers */

	Boolean		RULES=0,	/* true if use rulesets */
			PROBTHRESH=0;	/* true if to use soft thresholds */


/*************************************************************************/
/*									 */
/*	  Global data for See5/C5.0 used for building decision trees	 */
/*									 */
/*************************************************************************/


int
	Trial;		/* trial number for boosting */

float
	*ClassSum=0;	/* class weights during classification */


/*************************************************************************/
/*									 */
/*	Global data for constructing and applying rules			 */
/*									 */
/*************************************************************************/


CRule	*Rule=0;		/* current rules */

RuleNo	NRules;		/* number of rules */

int	RuleSpace;	/* space currently allocated for rules */

ClassNo	Default;	/* default class associated with ruleset */
