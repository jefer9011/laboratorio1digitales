/**
@file huffman_calc.c

@brief Ejercicio 02: Codificador huffman - Implementación - Calculo de códigos 
@author Oscar Diaz \<oddiaz001@ikasle.ehu.es\>

This file implements the huffman code calculator

*/

#include "huffman.h"

// STATIC MEMBERS
#ifdef HUFFMAN_ADT_USE_GLIB
gint huffman_nodecompare (gconstpointer a, gconstpointer b, gpointer user_data);
#else
hufadt_int huffman_nodecompare (const void * a, const void * b, void * user_data);
#endif

int huffman_conv_tree(nodelist_t* thelist, hufadt_tree_t* calctree, int cleancode);
void huffman_prune_tree(hufadt_tree_t basetree);

/**
 * @brief Calculate the huffman code for the current node list
 * @param thelist Pointer to a node list variable
 * @return 0 when successful, \< 0 on error
 */
int huffman_calc(nodelist_t* thelist)
{
	int retval = HUFFMAN_ERROR_OK;

	unsigned int tmpcode, tmpmask;
	
	hufadt_list_t treestack;
	hufadt_treenode_t *base_tree, *tree_parent, *tree_left, *tree_right;
	nodedata_t *nodetmp;
	
	if(thelist == NULL) {
		return HUFFMAN_ERROR_NULLP;
	}
	
	// calculate the equivalent tree
	
	retval = huffman_conv_tree(thelist, &base_tree, 1);
	
	if(retval == HUFFMAN_ERROR_OK) {
		// go through the entire tree and put the codes
		//worknode = hufadt_list_first(worklist);
		//tree_parent = hufadt_list_getdata(worknode);
		// make a stack from a list
		tree_parent = base_tree;
		treestack = hufadt_list_new(tree_parent);
		if(treestack == NULL) {
			retval = HUFFMAN_ERROR_DYNMEM;
		} else {
			tmpcode = 0;
			tmpmask = 1;
			// iterate on a non-empty stack
			for( ; hufadt_list_first(treestack) != NULL ; ) {
				// check parent in stack
				tree_parent = hufadt_list_getdata(hufadt_list_first(treestack));
				
				// check left: exists or already have a code
				tree_left = hufadt_tree_left(tree_parent);
				if(tree_left != NULL) {
					nodetmp = (nodedata_t*) hufadt_tree_getdata(tree_left);
					if(nodetmp->codebits == 0) {
						// new parent to stack
						treestack = hufadt_list_insert_first(treestack, tree_left);
						// dept the mask
						tmpmask += 1;
						// put code
						tmpcode <<= 1;
						tmpcode |= 0x1;
						// again
						continue;
					}
				}
				// check right
				tree_right = hufadt_tree_right(tree_parent);
				if(tree_right != NULL) {
					nodetmp = (nodedata_t*) hufadt_tree_getdata(tree_right);
					if(nodetmp->codebits == 0) {
						// new parent to stack
						treestack = hufadt_list_insert_first(treestack, tree_right);
						// dept the mask
						tmpmask += 1;
						// put code
						tmpcode <<= 1;
						// tmpcode |= 0x0; // put zero
						// again
						continue;
					}
				}
				// put code
				nodetmp = (nodedata_t*) hufadt_tree_getdata(tree_parent);
				nodetmp->code = tmpcode;
				nodetmp->codebits = tmpmask - 1;
				
				// checkpoint: the type in nodedata_t.code must be able to 
				// store all tmpcode without trim
				if((nodetmp->codebits / 8) > sizeof(fixed_integer)) {
					retval = HUFFMAN_ERROR_TYPES;
					// and let the algorithm end
				}
				
				// delete from stack
				treestack = hufadt_list_remove_first(treestack);

				// correct code
				tmpmask -= 1;
				tmpcode >>= 1;
			}
		}
	}
	
	// cleanup the temporal resources
	hufadt_tree_prune(base_tree);
	hufadt_tree_destroy(base_tree);
	return retval;
}

int huffman_conv_tree(nodelist_t* thelist, hufadt_tree_t* calctree, int cleancode)
{
	int retval = HUFFMAN_ERROR_OK;
	
	hufadt_list_t worklist;
	hufadt_listnode_t *worknode;
	hufadt_treenode_t *tree_parent, *tree_left, *tree_right;
	nodedata_t *symboltmp, *nodetmp;
	
	unsigned int funccode;
	
	if(thelist == NULL) {
		return HUFFMAN_ERROR_NULLP;
	}
	
	// work with a copy of the list
	worklist = hufadt_list_copy(thelist->symbol_list);
	
	// change the nodes from nodedata_t to hufadt_treenode_t trees 
	for(worknode = hufadt_list_first(worklist); worknode != NULL; worknode = hufadt_list_next(worknode)) {
		//nodetmp = ((nodedata_t*) worknode->data);
		nodetmp = (nodedata_t*) hufadt_list_getdata(worknode);
		// clean the code/mask if needed by caller
		if(cleancode){
			nodetmp->code = 0;
			nodetmp->codebits = 0;
		}
		//tree_parent = g_node_new(worknode->data);
		tree_parent = hufadt_tree_newnode(nodetmp);
		//worknode->data = tree_parent;
		hufadt_list_getdata(worknode) = tree_parent;
	}
	
	// build the tree: iterate while the list have more than one element
	while((hufadt_list_next(worklist)) != NULL) {
		
		// obtain two first elements from list
		worknode = hufadt_list_first(worklist);
		//tree_left = worknode->data;
		tree_left = hufadt_list_getdata(worknode);
		//worklist = hufadt_list_remove_node(worklist, worknode);
		worklist = hufadt_list_remove_first(worklist);
		worknode = hufadt_list_first(worklist);
		//tree_right = worknode->data;
		tree_right = hufadt_list_getdata(worknode);
		//worklist = hufadt_list_remove_node(worklist, worknode);
		worklist = hufadt_list_remove_first(worklist);

		// create a root tree data
		symboltmp = (nodedata_t *) malloc(sizeof(nodedata_t));
		if(symboltmp == NULL) {
			retval = HUFFMAN_ERROR_DYNMEM;
			break;
		}

		// assign the sum of probs from leafs to the root
		//nodetmp = tree_left->data;
		nodetmp = hufadt_tree_getdata(tree_left);
		symboltmp->probcount = nodetmp->probcount;
		//nodetmp = tree_right->data;
		nodetmp = hufadt_tree_getdata(tree_right);
		symboltmp->probcount += nodetmp->probcount;
		symboltmp->symbol = symboltmp->code = symboltmp->codebits = 0;
		
		// new tree
		tree_parent = hufadt_tree_new(symboltmp);
		if(tree_parent == NULL) {
			retval = HUFFMAN_ERROR_DYNMEM;
			break;
		}
		
		// left leaf (lesser prob)
		hufadt_tree_insert_left (tree_parent, tree_left);
		// right leaf (greater prob)
		hufadt_tree_insert_right (tree_parent, tree_right);
		
		// put back to list
		funccode = HUFFMAN_COMPARE_TREE;
		worklist = hufadt_list_insert_order(worklist, tree_parent, huffman_nodecompare, &funccode);
	}
	
	// delete the list element and leave only the tree
	tree_parent = hufadt_list_getdata(hufadt_list_first(worklist));
	worklist = hufadt_list_remove_first(worklist);
	
	// builded tree
	
	*calctree = tree_parent;
	
	return retval;
}
