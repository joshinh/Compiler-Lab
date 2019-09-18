		
%{
	#include <stdio.h>

	int yyerror(string);
	int yylex();

	extern int yylineno;  
	
	Procedure * proc;
	Procedure * newproc;
	Procedure * proccontext;

	int datatype;
	Data_Type ret_datatype;
	// int scope = 0;

	string procname;

	int local_offset = 0;
	int global_offset = 0;
	int formal_offset = 0;
	int decl_type = 0;

	int mainseen = 0;
	int inmain = 0;

	string empstr = "";

	Symbol_Table * glob_symtab = new Symbol_Table();
	
	void sanitycheck(Ast* ast){
		if(ast == NULL){
			cout << "hell is loose" << endl;
		}
	}
	
	void isLocalT(Symbol_Table * ent){
		if(ent ==  NULL){
			cout << "hell is loose" << endl;
		}
		else if((*ent).get_table_scope() == global){
			cout << "it's global" << endl;
		}
		else if((*ent).get_table_scope() == local){
			cout << "it's local" << endl;
		}
		else{
			cout << "arbit scope" << endl;
		}
	}

	
	void isLocal(Symbol_Table_Entry* ent){
		if(ent ==  NULL){
			cout << "hell is loose" << endl;
		}
		else if((*ent).get_symbol_scope() == global){
			cout << "it's global" << endl;
		}
		else if((*ent).get_symbol_scope() == local){
			cout << "it's local" << endl;
		}
		else{
			cout << "arbit scope" << endl;
		}
	}


	void setScope(Symbol_Table_Entry * ent, int curr_scope){
		if(curr_scope == 0){
			(*ent).set_symbol_scope(global);
		}
		else if(curr_scope == 1){
			(*ent).set_symbol_scope(local);
		}
		else if(curr_scope == 2){
			(*ent).set_symbol_scope(formal);
		}
		else{
			printf("cs316: Error : hell is loose : scope\n");
		}
	}

	void setScopeT(Symbol_Table * ent, int curr_scope){
		if(curr_scope == 0){
			(*ent).set_table_scope(global);
		}
		else if(curr_scope == 1){
			(*ent).set_table_scope(local);
		}
		else if(curr_scope == 2){
			(*ent).set_table_scope(formal);
		}
		else{
			printf("cs316: Error : hell is loose : scope\n");
		}
	}

	void debugPrint(string a){
		cout<< "Debug error:"<< a <<"\n";
	}

%}

%start program

%union{
	int integer_value;
	double double_value;
	string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	Sequence_Ast * seq_ast;	
}

/* token spec */
%token <string_value> NAME
%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token RETURN
%token INTEGER
%token FLOAT
%token VOID
%token ASSIGN
%token WHILE
%token DO
%token IF
%token ELSE
%token LTE
%token GTE
%token EQUAL
%token NE
%token LT
%token GT
%token AND
%token OR
%token NOT
%token PRINT

/* type spec */
%type <seq_ast> body_statement_list
%type <ast_list> statement_list param_list
%type <ast> open_statement closed_statement cond_statement cond_statement1 cond_final rel_exp statement arith_statement arith_statement1 arith_statement2 variable constant final 
%type <symbol_table> local_declarations variable_declaration_list variable_declaration variable_list arg_list

%%

program :		{(*glob_symtab).set_table_scope(global);}
			global_declarations procedure_definition_list 
				{
					if (mainseen == 0)
					{
						printf("cs316: Error: in parser: main procedure not found (hahaha!)\n");
						exit(0);
					}
				}
			;

global_declarations	 		: 	global_declarations glob_variable_declaration ';'
								{
									program_object.set_global_table(*glob_symtab);
								}
							|	global_declarations glob_proc_declaration ';'
								{;}
							| 	%empty
								{;}
							;

glob_variable_declaration	:	ret_type glob_variable_list
								{
									if(ret_datatype == void_data_type){
										printf("cs316: Error: VOID datatype object initialized\n");
										exit(0);
									}
								}
							;


glob_variable_list			:	NAME
							{
																		
								Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$1).append("_"), ret_datatype, yylineno);
								setScope(newvar, global);
								newvar->set_start_offset(global_offset);
								if(ret_datatype == int_data_type){
									global_offset = global_offset - 4;
								}
								else if (ret_datatype == double_data_type){
									global_offset = global_offset - 8;
								}
								else{
									printf("cs316: Error: non int/double datatype object initialized\n");
								}
								newvar->set_end_offset(global_offset);
								(*glob_symtab).push_symbol(newvar);

								                                
                      		}

							| 	glob_variable_list ',' NAME
							
							{
                				Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$3).append("_"), ret_datatype, yylineno);
								setScope(newvar, global);
								newvar->set_start_offset(global_offset);
								if(ret_datatype == int_data_type){
									global_offset = global_offset - 4;
								}
								else if (ret_datatype == double_data_type){
									global_offset = global_offset - 8;
								}
								else{
									printf("cs316: Error: non int/double datatype object initialized\n");
								}
								newvar->set_end_offset(global_offset);
								(*glob_symtab).push_symbol(newvar);
                    			                    			
                    		}
                        	;


glob_proc_declaration		:	ret_type NAME '(' arg_list ')'
								{
									if ((*$2).compare("main") == 0)
									{
										printf("cs316: Error: main procedure declaration not permitted\n");
										exit(0);
									}
									(*$2).append("_");
									newproc = new Procedure(ret_datatype, *$2, yylineno);
									newproc->set_formal_param_list(*$4);
									// newproc->set_return_type(ret_datatype);
									program_object.set_proc_to_map(*$2, newproc);
									formal_offset = 0;
									decl_type = 0;
								}
							;	                        	



arg_list		:	arg_list ',' type NAME 
					{
						if(decl_type != 1){
							printf("cs316: Error: in arg_list non-uniform argument declaration\n");
							exit(0);
						}
						(*$4).append("_");
						
						if(($1)->variable_in_formal_list_check(*$4)){
							printf("cs316: Error: in arg_list variable is declared twice\n");
							exit(0);
						}

						Symbol_Table_Entry * newarg;
						if (datatype == INTEGER){
							newarg = new Symbol_Table_Entry(*$4, int_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 4;
							newarg->set_end_offset(formal_offset);
						}
						else if (datatype == FLOAT){
							newarg = new Symbol_Table_Entry(*$4, double_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 8;
							newarg->set_end_offset(formal_offset);
						}
						else{
							printf("cs316: Error : void datatype symbol\n");
						}
						(*$1).push_symbol(newarg);
						$$ = $1;			
					}
				|	arg_list ',' type
					{	
						if(decl_type != 2){
							printf("cs316: Error: in arg_list non-uniform argument declaration\n");
							exit(0);
						}

						Symbol_Table_Entry * newarg;
						if (datatype == INTEGER){
							newarg = new Symbol_Table_Entry(empstr, int_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 4;
							newarg->set_end_offset(formal_offset);
						}
						else if (datatype == FLOAT){
							newarg = new Symbol_Table_Entry(empstr, double_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 8;
							newarg->set_end_offset(formal_offset);
						}
						else{
							printf("cs316: Error : void datatype symbol\n");
						}
						(*$1).push_symbol(newarg);
						$$ = $1;		
					}
				|	type
					{
						decl_type = 2;

						Symbol_Table * arg_symtab = new Symbol_Table();
						Symbol_Table_Entry * newarg;
						if (datatype == INTEGER){
							newarg = new Symbol_Table_Entry(empstr, int_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 4;
							newarg->set_end_offset(formal_offset);
						}
						else if (datatype == FLOAT){
							newarg = new Symbol_Table_Entry(empstr, double_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 8;
							newarg->set_end_offset(formal_offset);
						}
						else{
							printf("cs316: Error : void datatype symbol\n");
						}
						(*arg_symtab).push_symbol(newarg);
						$$ = arg_symtab;	
					}
				|	type NAME
					{
						decl_type = 1;

						(*$2).append("_");
						Symbol_Table * arg_symtab = new Symbol_Table();
						Symbol_Table_Entry * newarg;
						if (datatype == INTEGER){
							newarg = new Symbol_Table_Entry(*$2, int_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 4;
							newarg->set_end_offset(formal_offset);
						}
						else if (datatype == FLOAT){
							newarg = new Symbol_Table_Entry(*$2, double_data_type, yylineno, sp_ref);
							setScope(newarg, formal);
							newarg->set_start_offset(formal_offset);
							formal_offset = formal_offset + 8;
							newarg->set_end_offset(formal_offset);
						}
						else{
							printf("cs316: Error : void datatype symbol\n");
						}
						(*arg_symtab).push_symbol(newarg);
						$$ = arg_symtab;

					}
				|	%empty
					{
						Symbol_Table * arg_symtab = new Symbol_Table();
						$$ = arg_symtab;
					}
				;	




variable_declaration_list	:	variable_declaration 
								{

									$$ = $1;
									
								}
                			|	variable_declaration_list variable_declaration
            					{
            						
            						(*$1).append_list(*$2, yylineno);
            						$$ = $1;
            					}
                			;

variable_declaration		:	INTEGER
								{
									datatype = INTEGER;
								}
								variable_list ';'
								{
									$$ = $3;
								}
							|	FLOAT 
								{
									datatype = FLOAT;
								}
								variable_list ';'
								{
									$$ = $3;	
								}
				            ;

variable_list           	: 	NAME	
								{
									if(datatype == INTEGER){
																			
										Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$1).append("_"), int_data_type, yylineno);

										setScope(newvar, local);
										
										newvar->set_start_offset(local_offset);
										local_offset = local_offset - 4;
										newvar->set_end_offset(local_offset);	
									
										Symbol_Table * symtab = new Symbol_Table();
										(*symtab).push_symbol(newvar);

										setScopeT(symtab, local);
										
										$$ = symtab;
										// Eval_Result_Value_Int * r = new Eval_Result_Value_Int();
										// interpreter_global_table.put_variable_value(*r,*$1);

									}
									else if(datatype == FLOAT){
										Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$1).append("_"), double_data_type, yylineno);
										
										setScope(newvar, local);

										
										newvar->set_start_offset(local_offset);
										local_offset = local_offset - 8;
										newvar->set_end_offset(local_offset);	
									
										Symbol_Table * symtab = new Symbol_Table();

										(*symtab).push_symbol(newvar);
										setScopeT(symtab, local);
										
										$$ = symtab;
										//Eval_Result_Value_Double * r = new Eval_Result_Value_Double();
										//interpreter_global_table.put_variable_value(*r,*$1);

										// isLocalT($$);
									}
									else{
										printf("cs316: Error : void datatype symbol\n");
									}
									
                                
                          		}
                   	     	| 	variable_list ',' NAME
                    	    	{
                        			if(datatype == INTEGER){
										Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$3).append("_"), int_data_type, yylineno);
										
										newvar->set_start_offset(local_offset);
										local_offset = local_offset - 4;
										newvar->set_end_offset(local_offset);	
									
										setScope(newvar, local);
										
										(*$1).push_symbol(newvar);
                        				$$ = $1;
                        				
                        			}
									else if(datatype == FLOAT){
										Symbol_Table_Entry * newvar = new Symbol_Table_Entry((*$3).append("_"), double_data_type, yylineno);
										
										newvar->set_start_offset(local_offset);
										local_offset = local_offset - 8;
										newvar->set_end_offset(local_offset);	
									
										setScope(newvar, local);
										
										(*$1).push_symbol(newvar);
										$$ = $1;
                        			}
									else{
										printf("cs316: Error : void datatype symbol\n");
									}
                        		}
		                	;

procedure_definition_list	:	procedure_definition_list procedure_definition
								{;}
							|	procedure_definition
								{;}
							;

procedure_definition  		:	ret_type NAME '(' arg_list ')' '{' local_declarations
                    			{
                    				// list<Symbol_Table_Entry *> al = ($4)->get_table();
                    				// cout << "size " << al.size() << endl;
                    				// list<Symbol_Table_Entry *> bl = ($7)->get_table();
                    				// cout << "size " << bl.size() << endl;


                    				if ((*$2).compare("main") == 0)
                    				{
                    					if (mainseen == 1){
                    						printf("cs316: Error: mulitple main procedures found\n");
                    						exit(0);
                    					}
                    					if (ret_datatype != void_data_type)
                    					{
                    						printf("cs316: Error: non-void main datatype\n");
                    						exit(0);
                    					}
                    					if (!(($4)->is_empty())){
                    						printf("cs316: Error: non-zero arguments in main procedure\n");
                    						exit(0);
                    					}
                    					mainseen = 1;
                    					inmain = 1;
                    					proc = new Procedure(void_data_type, *$2, yylineno);
                          				proc->set_local_list(*$7);
                          				program_object.set_proc_to_map("main", proc);
                          				procname = *$2;
                          				proccontext = proc;
                    				}
                    				else{
                    					(*$2).append("_");
                    					if (program_object.is_procedure_exists((*$2))){
                    						
                    						Procedure * insproc = program_object.get_procedure_prototype((*$2));
                    						if (insproc->is_proc_defined()){
		                    					printf("cs316: Error: in parser: function already defined\n");
		                    					exit(0);
		                    				}
											list<Symbol_Table_Entry *> * arglist = (list<Symbol_Table_Entry *> *) &($4)->get_table();

                    						// if (inslist->size() != arglist->size()){
                    						// 	printf("cs316: Error: invalid function signature in procedure_definition - arguments do not match\n");
                    						// }
                    						if (ret_datatype != insproc->get_return_type()){
                    							printf("cs316: Error: invalid function signature in procedure_definition - return type does not match\n");
                    						}

                    						int len = arglist->size();
                    						for (int i = 0; i < len; i++){
                    							// Symbol_Table_Entry insentry = instable->get_symbol_table_entry_by_index(i+1);
                    							Symbol_Table_Entry argentry = ($4)->get_symbol_table_entry_by_index(i+1);
                    							if (argentry.get_variable_name().compare("") == 0){
                    								printf("cs316: Error: empty argument in procedure definition\n");
                    								exit(0);
                    							}
                    							// if (argentry.get_data_type() != insentry.get_data_type()){
                    								// printf("cs316: Error: invalid function signature in procedure_definition - argument datatype does not match\n");
                    								// exit(0);	
                    							// }
                    						}
                    						insproc->set_formal_param_list(*$4);
                    						insproc->set_local_list(*$7);
                    						insproc->set_proc_is_defined();
                    						proccontext = insproc;
                    					}
                    					else{
                    						list<Symbol_Table_Entry *> * arglist = (list<Symbol_Table_Entry *> *) &($4)->get_table();
                    						int len = arglist->size();
                    						for (int i = 0; i < len; i++){
                    							Symbol_Table_Entry & argentry = ($4)->get_symbol_table_entry_by_index(i+1);
                    							if (argentry.get_variable_name().compare("") == 0){
                    								printf("cs316: Error: empty argument in procedure definition\n");
                    								exit(0);
                    							}
                    						}
                    						newproc = new Procedure(ret_datatype, *$2, yylineno);
											newproc->set_formal_param_list(*$4);
											newproc->set_local_list(*$7);
											newproc->set_proc_is_defined();
											// newproc->set_return_type(ret_datatype);
											program_object.set_proc_to_map(*$2, newproc);
                    						proccontext = newproc;
                    					}
                    					
                    				}
                    				procname = *$2;
                    				formal_offset = 0;
                    			}
                        		statement_list '}'
                    			{
                    				proccontext->set_ast_list(*$9);
                    				local_offset = 0;
                    				if (inmain == 1){
                    					// (*proc).set_ast_list(*$9);  
                    	    			//program_object.set_procedure(proc, yylineno);
                    	    			inmain = 0;
                    				}
                    			}
                      		;

local_declarations			:	variable_declaration_list
								{
									$$ = $1;
                    			}
							|	%empty
								{
									Symbol_Table * symtab = new Symbol_Table();
									setScopeT(symtab, local);			
									$$ = symtab;
                    			}
							;
	

statement_list		: 	%empty
						{
                    		list<Ast *> * astlist = new list<Ast *>();
                        	$$ = astlist;
                    	}
					|	statement_list
						statement
						{
							(*$1).push_back($2);
                  			$$ = $1;
                  		}
					;

statement		:	open_statement
						{
							// new addition: 8/3
							$$ = $1;
						}
				|	closed_statement
						{
							// new addition: 8/3
							$$ = $1;
							//debugPrint("closed_statement");
						}
				;



closed_statement 	:	variable ASSIGN arith_statement ';'
						{
	  						if((*$1).get_data_type() == (*$3).get_data_type()){
		  						Assignment_Ast * temp_ass_ast = new Assignment_Ast($1, $3, yylineno);
		  						$$ = temp_ass_ast;
	  						}
	  						else{
	  							printf("cs316: Error : Data type doesn't match!\n");
	  							exit(0);
	  						}
	  					}
	  				|	PRINT variable ';'
	  					{
	  						Print_Ast * past = new Print_Ast($2, yylineno);
	  						$$ = past;
	  					}
					|	WHILE '(' cond_statement ')' closed_statement
						{
							// new addition: 8/3
							Iteration_Statement_Ast * while_ast = new Iteration_Statement_Ast($3, $5, yylineno, false);
							$$ = while_ast;
							(*$$).set_data_type(void_data_type);
						}
					|	DO closed_statement WHILE '(' cond_statement ')' ';'
						{
							// new addition: 8/3
							Iteration_Statement_Ast * while_ast = new Iteration_Statement_Ast($5, $2, yylineno, true);
							//cout << while_ast->check_ast() << endl;
							$$ = while_ast;
							(*$$).set_data_type(void_data_type);
						}
					|	IF '(' cond_statement ')' closed_statement ELSE closed_statement
						{
							// new addition: 8/3
							Selection_Statement_Ast * if_ast = new Selection_Statement_Ast($3, $5, $7, yylineno);
							$$ = if_ast;
							(*$$).set_data_type(void_data_type);
						}
					|	'{' body_statement_list '}'
						{
							// new addition: 8/3
							$$ = $2;
						}
					|	RETURN arith_statement ';'
						{
							if (proccontext->get_return_type() != ($2)->get_data_type())
							{
								printf("cs316: Error: return type doesn't match with actual return value\n");
								exit(0);
							}
							Return_Ast * retast = new Return_Ast($2, procname, yylineno);
							$$ = retast;
						}
					|	RETURN ';'
						{
							if (proccontext->get_return_type() != void_data_type){
								printf("cs316: Error: empty return from non-void function\n");
								exit(0);
							}
							Return_Ast * retast = new Return_Ast(NULL, procname, yylineno);
							$$ = retast;
						}

					|	variable ASSIGN NAME '(' param_list ')' ';'
						{
							(*$3).append("_");
							if (!program_object.is_procedure_exists(*$3)){
								printf("cs316: Error: procedure not declared\n");
								exit(0);
							}

							Procedure * currproc = program_object.get_procedure_prototype(*$3);

							if ((*$1).get_data_type() != currproc->get_return_type()){
								printf("cs316: Error: in procedure called return type does not match\n");
								exit(0);
							}
							long t = 0;
							Call_Ast * callast = new Call_Ast(*$3, yylineno);
							callast->set_register((Register_Descriptor *) t);
							callast->set_actual_param_list(*$5);
							callast->check_actual_formal_param(currproc->get_formal_param_list());
							Assignment_Ast * assnast = new Assignment_Ast($1, callast, yylineno);
							$$ = assnast;
						}
					|	NAME '(' param_list ')' ';'
						{
							(*$1).append("_");
							if (!program_object.is_procedure_exists(*$1)){
								printf("cs316: Error: procedure not declared\n");
								exit(0);
							}
							Procedure * currproc = program_object.get_procedure_prototype(*$1);

							//Must be void return type

							// if (!currproc->is_proc_defined()){
							// 	printf("cs316: Error: procedure not defined\n");
							// 	exit(0);
							// }

							// if (currproc->get_return_type() != void_data_type){
							// 	printf("cs316: Error: in procedure return type must be void\n");
							// 	exit(0);
							// }
							long t = 1;
							Call_Ast * callast = new Call_Ast(*$1, yylineno);
							callast->set_register((Register_Descriptor *) t);
							callast->set_actual_param_list(*$3);
							callast->check_actual_formal_param(currproc->get_formal_param_list());
							$$ = callast;

						}
					;

param_list 		:	param_list ',' arith_statement
					{
						(*$1).push_back($3);
                  		$$ = $1;
					}
				| 	arith_statement
					{
						list<Ast *> * plist = new list<Ast *>();
						plist->push_back($1);
						$$ = plist;	
					}	
				|	%empty
					{
						list<Ast *> * plist = new list<Ast *>();
						$$ = plist;
					}
				;

open_statement	:	IF '(' cond_statement ')' closed_statement ELSE open_statement
						{
							// new addition: 8/3
							Selection_Statement_Ast * if_ast = new Selection_Statement_Ast($3, $5, $7, yylineno);
							$$ = if_ast;
							(*$$).set_data_type(void_data_type);
						}
				|	IF '(' cond_statement ')' statement
						{
							// debugPrint("if done");
							// new addition: 8/3
							Selection_Statement_Ast * if_ast = new Selection_Statement_Ast($3, $5, NULL, yylineno);
							//cout<< if_ast->check_ast() << endl;
							$$ = if_ast;
							(*$$).set_data_type(void_data_type);		
						}
				|	WHILE '(' cond_statement ')' open_statement
						{
							// new addition: 8/3
							Iteration_Statement_Ast * while_ast = new Iteration_Statement_Ast($3, $5, yylineno, false);
							$$ = while_ast;
							(*$$).set_data_type(void_data_type);
						}
				;

body_statement_list	:	statement
						{
							// new addition: 8/3
							Sequence_Ast * seq_ast = new Sequence_Ast(yylineno);
							(*seq_ast).ast_push_back($1);
							$$ = seq_ast;
						}
					|	body_statement_list
						statement
						{
							// new addition: 8/3
							(*$1).ast_push_back($2);
							$$ = $1;
						}
					;

cond_statement	:	cond_statement OR cond_statement1
					{
						// new addition: 8/3
						Logical_Expr_Ast * log_ast = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
						$$ = log_ast;
						(*$$).set_data_type(void_data_type);
					}
				|	cond_statement1
					{
						$$ = $1;
					}
				;

cond_statement1	:	cond_statement1 AND cond_final
					{
						// new addition: 8/3
						Logical_Expr_Ast * log_ast = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
						$$ = log_ast;
						(*$$).set_data_type(void_data_type);
					}
				| 	cond_final
					{
						$$ = $1;
					}
				;

cond_final		:	NOT cond_final
					{
						// new addition: 8/3
						Logical_Expr_Ast * log_ast = new Logical_Expr_Ast(NULL, _logical_not, $2, yylineno);
						$$ = log_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	'(' cond_statement ')'
					{
						$$ = $2;
					}
				|	rel_exp
					{
						// debugPrint("rel_exp");
						$$ = $1;
					}
				;

rel_exp			:	arith_statement1 LTE arith_statement
					{
						// new addition: 8/3
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	arith_statement1 LT arith_statement
					{
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, less_than, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	arith_statement1 GTE arith_statement
					{
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	arith_statement1 GT arith_statement
					{
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	arith_statement1 NE arith_statement
					{
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				|	arith_statement1 EQUAL arith_statement
					{
						Relational_Expr_Ast * rel_ast = new Relational_Expr_Ast($1, equalto, $3, yylineno);
						$$ = rel_ast;
						//(*$$).set_data_type(void_data_type);
					}
				;


variable		:	NAME
					{
                		string temp_string = (*$1).append("_");
                		Symbol_Table_Entry oldentry;
                		Symbol_Table_Entry * newvar;
                		if((*proccontext).variable_in_symbol_list_check(temp_string)){
                			oldentry = (*proccontext).get_symbol_table_entry(temp_string);
                			newvar = new Symbol_Table_Entry(temp_string, oldentry.get_data_type(), oldentry.get_lineno());
                			newvar->set_start_offset(oldentry.get_start_offset());
                			newvar->set_end_offset(oldentry.get_end_offset());
                			setScope(newvar, oldentry.get_symbol_scope());	
                		}
                		else if((*proccontext).variable_in_formal_list_check(temp_string)){
                			oldentry = (*proccontext).get_formal_param_entry(temp_string);
                			newvar = new Symbol_Table_Entry(temp_string, oldentry.get_data_type(), oldentry.get_lineno(), fp_ref);
                			newvar->set_start_offset(oldentry.get_start_offset());
                			newvar->set_end_offset(oldentry.get_end_offset());
                			setScope(newvar, oldentry.get_symbol_scope());
                		}
                    	else if(program_object.variable_in_symbol_list_check(temp_string)){
                			oldentry = program_object.get_symbol_table_entry(temp_string);
                			newvar = new Symbol_Table_Entry(temp_string, oldentry.get_data_type(), oldentry.get_lineno());
                			newvar->set_start_offset(oldentry.get_start_offset());
                			newvar->set_end_offset(oldentry.get_end_offset());
                			setScope(newvar, oldentry.get_symbol_scope());
                		}                		
                		else{
                			printf("cs316: Error : Data not present in either of global or local tables\n");
                			exit(0);
                		}
                		
                    	Name_Ast * temp_name_ast = new Name_Ast(temp_string, *newvar, yylineno);
                    	$$ = temp_name_ast;


                	}
				;

constant		:	INTEGER_NUMBER
					{
                		Number_Ast<int> *temp_num_ast = new Number_Ast<int>($1, int_data_type, yylineno);
                   		$$ = temp_num_ast;
                	}
				| 	DOUBLE_NUMBER
                	{
                		Number_Ast<double> *temp_num_ast = new Number_Ast<double>($1, double_data_type, yylineno);
                    	$$ = temp_num_ast;
                	}
				;

arith_statement :	cond_statement '?' arith_statement ':' arith_statement  
					{
						Conditional_Expression_Ast * cond_ast = new Conditional_Expression_Ast($1, $3, $5, yylineno);
						if((*$3).get_data_type() != (*$5).get_data_type()){
                    		printf("cs316: Error : in Cond_Ast, data type doesn't match!\n");
                    		exit(0);
                    	}
						$$ = cond_ast;
					}
				|	arith_statement1
					{
						$$ = $1;
					}
				;	


arith_statement1 :	arith_statement1 '+' arith_statement2
					{
						if ((*$3).is_value_zero())
						{
							Plus_Ast * plus = new Plus_Ast($1, $3, yylineno);
                        	$$ = plus;
                        	(*$$).set_data_type((*$1).get_data_type());
						}
						else if((*$1).is_value_zero()){
							Plus_Ast * plus = new Plus_Ast($1, $3, yylineno);
                        	$$ = plus;
                        	(*$$).set_data_type((*$3).get_data_type());
						}
						else if((*$1).get_data_type() != (*$3).get_data_type()){
	                    		printf("cs316: Error : in Plus_Ast, data type doesn't match!\n");
	                    		exit(0);
	                    }
                    	else{
                    		Plus_Ast * plus = new Plus_Ast($1, $3, yylineno);
                        	$$ = plus;
                        	(*$$).set_data_type((*$1).get_data_type());
                    	}
                    }
				|	arith_statement1 '-' arith_statement2
                	{
                		if ((*$3).is_value_zero())
						{
							Minus_Ast * sub = new Minus_Ast($1, $3, yylineno);
                        	$$ = sub;
                        	(*$$).set_data_type((*$1).get_data_type());
						}
						else if((*$1).is_value_zero()){
							Minus_Ast * sub = new Minus_Ast($1, $3, yylineno);
                        	$$ = sub;
                        	(*$$).set_data_type((*$3).get_data_type());
						}
						else if((*$1).get_data_type() != (*$3).get_data_type()){
	                    		printf("cs316: Error : in Minus_Ast, data type doesn't match!\n");
	                    		exit(0);
	                    }
                    	else{
                    		Minus_Ast * sub = new Minus_Ast($1, $3, yylineno);
                        	$$ = sub;
                        	(*$$).set_data_type((*$1).get_data_type());
                    	}

                	}
				|	arith_statement2
                	{
                    	$$ = $1;
                    }
				;

arith_statement2 :	arith_statement2 '*' final
					{
						if ((*$3).is_value_zero())
						{
							Mult_Ast * mult = new Mult_Ast($1, $3, yylineno);
	                        $$ = mult;
	                        (*$$).set_data_type((*$1).get_data_type());
						}
						else if((*$1).is_value_zero()){
							Mult_Ast * mult = new Mult_Ast($1, $3, yylineno);
                        	$$ = mult;
                        	(*$$).set_data_type((*$3).get_data_type());
						}
						else if((*$1).get_data_type() != (*$3).get_data_type()){
	                    		printf("cs316: Error : in Mult_Ast, data type doesn't match!\n");
	                    		exit(0);
	                    }
                    	else{
                    		Mult_Ast * mult = new Mult_Ast($1, $3, yylineno);
	                        $$ = mult;
	                        (*$$).set_data_type((*$1).get_data_type());
                    	}

                    }
				 | 	arith_statement2 '/' final
                 	{
						if ((*$3).is_value_zero())
						{
            	        	Divide_Ast * div = new Divide_Ast($1, $3, yylineno);
                	        $$ = div;
	                        (*$$).set_data_type((*$1).get_data_type());
						}
						else if((*$1).is_value_zero()){
    	                	Divide_Ast * div = new Divide_Ast($1, $3, yylineno);
        	                $$ = div;
                        	(*$$).set_data_type((*$3).get_data_type());
						}
						else if((*$1).get_data_type() != (*$3).get_data_type()){
	                    		printf("cs316: Error : in Div_Ast, data type doesn't match!\n");
	                    		exit(0);
	                    }
                    	else{
	                    	Divide_Ast * div = new Divide_Ast($1, $3, yylineno);
	                        $$ = div;
	                        (*$$).set_data_type((*$1).get_data_type());
                    	}

                    }
				 |  final
                 	{
                    	$$ = $1;
                    }
				 ;

final	:	'-' final
			{
				UMinus_Ast * umast = new UMinus_Ast($2, NULL, yylineno);
				$$ = umast;
				(*$$).set_data_type((*$2).get_data_type());
			}
		|	'(' arith_statement ')'
			{
				$$ = $2;
			}
		|	variable 	
        	{
            	$$ = $1;
            }
		|	constant
        	{
            	$$ = $1;
            }
		;			


type    : INTEGER {datatype = INTEGER;}
	    | FLOAT  {datatype = FLOAT;}
	    ;

ret_type	: INTEGER {ret_datatype = int_data_type;}
			| FLOAT	{ret_datatype = double_data_type;}
			| VOID {ret_datatype = void_data_type;}
			;


%%

int yyerror(string mesg)
{
	cout << "cs316: Error: " << mesg << endl;
}

