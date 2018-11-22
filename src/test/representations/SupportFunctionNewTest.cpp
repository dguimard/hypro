/**
 * Tests for new SupportFunction architecture
 *
 * @author Phillip Tse
 */ 

#include "../../hypro/representations/GeometricObject.h"
#include "gtest/gtest.h"
#include "../defines.h"

using namespace hypro;

template<typename Number>
class SupportFunctionNewTest : public ::testing::Test {
protected:
	virtual void SetUp() {

		////Construct leaf nodes
		//box1 = Box<TypeParam>(std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
		//box2 = Box<TypeParam>(std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(0)}), Point<TypeParam>({TypeParam(2), TypeParam(2)})));
		////Assemble them to a tree 
		//sfl1 = SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>(&box1);
		//sfl2 = SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>(&box2);
		////Build trafop
		//matrix_t<TypeParam> trafoMat = matrix_t<TypeParam>::Identity(2,2);
		//vector_t<TypeParam> trafoVec = vector_t<TypeParam>::Zero(2);
		//sfWithTrafo = sfl1.affineTransformation(trafoMat, trafoVec);
		////Build sum operation as root 
		//sum = sfWithTrafo.minkowskiSum(sf2);
		
	}

	virtual void TearDown() {}	

	//Box<TypeParam> box1;
	//Box<TypeParam> box2;
	//SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf1;
	//SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf2;
	//SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfWithTrafo;
	//SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sum;

};

////// Data Structure Tests

TYPED_TEST(SupportFunctionNewTest, LeafTest){

	Box<TypeParam> box;
	matrix_t<TypeParam> directions = matrix_t<TypeParam>::Identity(2,2);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf(&box);
	EXPECT_TRUE(sf.getRoot()->getType() == SFNEW_TYPE::LEAF);
	EXPECT_EQ(sf.getRoot()->getOriginCount(), 0);
	EXPECT_EQ(sf.getRoot()->getChildren().size(), 0);
	EXPECT_EQ(sf.getRoot().use_count(), 1);
	EXPECT_EQ(*(dynamic_cast<Leaf<TypeParam,SupportFunctionNewDefault,Box<TypeParam>>*>(sf.getRoot().get())->getRepresentation()), box);

	//Change representation of same type
	Box<TypeParam> box2 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
	dynamic_cast<Leaf<TypeParam,SupportFunctionNewDefault,Box<TypeParam>>*>(sf.getRoot().get())->setRepresentation(&box2);
	EXPECT_EQ(*(dynamic_cast<Leaf<TypeParam,SupportFunctionNewDefault,Box<TypeParam>>*>(sf.getRoot().get())->getRepresentation()), box2);
	
/*	WAIT UNTIL VPOLY::MULTIEVALUATE IMPLEMENTED
	VPolytope<TypeParam> vpoly;
	Leaf<TypeParam, VPolytope<TypeParam>> child(&vpoly);
	r.addToChildren(&child);
	EXPECT_TRUE(r.getChildren().size() == 1);
	EXPECT_TRUE(child.getChildren().size() == 0);
*/
}


//Have a box as a leaf. Put a chain of trafoOps above the leaf. 
//Check currentExponent, successiveTransformations and parameter values
TYPED_TEST(SupportFunctionNewTest, TrafoOp){

	//Make the box 
	Point<TypeParam> p({TypeParam(0),TypeParam(0)});
	Point<TypeParam> q({TypeParam(1),TypeParam(1)});
	Box<TypeParam> box(std::make_pair(p,q));
	
	//The tree with only one leaf containing the box
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf(&box);
	EXPECT_TRUE(sf.getRoot()->getType() == SFNEW_TYPE::LEAF);
	EXPECT_TRUE(sf.getRoot()->getOriginCount() == 0);
	EXPECT_TRUE(sf.getRoot()->getChildren().size() == 0);

	//tMat = 2 * Identity_Matrix , tVec = (1 0 0)
	int dim = 2;
	matrix_t<TypeParam> tMat = matrix_t<TypeParam>::Identity(dim, dim);
	vector_t<TypeParam> tVec = vector_t<TypeParam>::Zero(dim);
	tVec(0) = 1;

	//Directions to evaluate, is the identity matrix
	matrix_t<TypeParam> directions = matrix_t<TypeParam>::Identity(dim,dim);

	//Save pointer to parameters for later, checking if it remains the same object 
	std::shared_ptr<const LinTrafoParameters<TypeParam,SupportFunctionNewDefault>> trafo0Params = nullptr;

	//sf1
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf1 = sf.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf1.getRoot() != nullptr);
	EXPECT_TRUE(sf1.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf1.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf1.getRoot().get()))->getSuccessiveTransformations(), 0);
	std::cout << "Constructed trafo1" << std::endl;

	//Fill trafo0Params
	trafo0Params = (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf1.getRoot().get()))->getParameters();
	std::cout << "saved trafo0Params with address " << trafo0Params << std::endl;

	//sf2
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf2 = sf1.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf2.getRoot() != nullptr);
	EXPECT_TRUE(sf2.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf2.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf2.getRoot().get()))->getSuccessiveTransformations(), 1);
	std::cout << "Constructed trafo2" << std::endl;

	//Check whether parameter object remained the same (whether both point to the same address)
	EXPECT_EQ(trafo0Params, (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf2.getRoot().get())->getParameters()));

	//Evaluate
	std::cout << "START EVALUATION FOR 2 TRAFOS\n"; 
	std::vector<EvaluationResult<TypeParam>> res = sf2.multiEvaluate(directions,true);
	std::cout << "END EVALUATION\n";

	//Check if the supportvalues were right
	//Should be [1 + #trafos , 1] everywhere
	EXPECT_EQ(res.at(0).supportValue, TypeParam(3));
	EXPECT_EQ(res.at(1).supportValue, TypeParam(1));

	//sf3
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf3 = sf2.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf3.getRoot() != nullptr);
	EXPECT_TRUE(sf3.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf3.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf3.getRoot().get()))->getSuccessiveTransformations(), 2);
	std::cout << "Constructed trafo3" << std::endl;

	//sf4
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf4 = sf3.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf4.getRoot() != nullptr);
	EXPECT_TRUE(sf4.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf4.getRoot().get()))->getCurrentExponent(), 4);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf4.getRoot().get()))->getSuccessiveTransformations(), 0);
	std::cout << "Constructed trafo4" << std::endl;

	//Check whether parameter object remained the same (whether both point to the same address)
	EXPECT_EQ(trafo0Params, (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf4.getRoot().get())->getParameters()));

	//Evaluate
	std::cout << "START EVALUATION FOR 4 TRAFOS\n"; 
	res = sf4.multiEvaluate(directions,true);
	std::cout << "END EVALUATION\n";

	//Check if the supportvalues were right
	//Should be [1 + #trafos , 1] everywhere
	EXPECT_EQ(res.at(0).supportValue, TypeParam(5));
	EXPECT_EQ(res.at(1).supportValue, TypeParam(1));

	//sf5
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf5 = sf4.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf5.getRoot() != nullptr);
	EXPECT_TRUE(sf5.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf5.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf5.getRoot().get()))->getSuccessiveTransformations(), 0);
	std::cout << "Constructed trafo5" << std::endl;

	//sf6
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf6 = sf5.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf6.getRoot() != nullptr);
	EXPECT_TRUE(sf6.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf6.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf6.getRoot().get()))->getSuccessiveTransformations(), 1);
	std::cout << "Constructed trafo6" << std::endl;

	//sf7
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf7 = sf6.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf7.getRoot() != nullptr);
	EXPECT_TRUE(sf7.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf7.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf7.getRoot().get()))->getSuccessiveTransformations(), 2);
	std::cout << "Constructed trafo7" << std::endl;

	//sf8
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf8 = sf7.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf8.getRoot() != nullptr);
	EXPECT_TRUE(sf8.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf8.getRoot().get()))->getCurrentExponent(), 4);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf8.getRoot().get()))->getSuccessiveTransformations(), 1);
	std::cout << "Constructed trafo8" << std::endl;

	//Check whether parameter object remained the same (whether both point to the same address)
	EXPECT_EQ(trafo0Params, (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf8.getRoot().get())->getParameters()));

	//Evaluate
	std::cout << "START EVALUATION FOR 8 TRAFOS\n"; 
	res = sf8.multiEvaluate(directions,true);
	std::cout << "END EVALUATION\n";

	//Check if the supportvalues were right
	//Should be [1 + #trafos , 1] everywhere
	EXPECT_EQ(res.at(0).supportValue, TypeParam(9));
	EXPECT_EQ(res.at(1).supportValue, TypeParam(1));

	//sf9
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf9 = sf8.affineTransformation(tMat, tVec); 
	EXPECT_TRUE(sf9.getRoot() != nullptr);
	EXPECT_TRUE(sf9.getRoot()->getChildren().size() == 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf9.getRoot().get()))->getCurrentExponent(), 1);
	EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf9.getRoot().get()))->getSuccessiveTransformations(), 0);
	std::cout << "Constructed trafo9" << std::endl;

	//Check whether parameter object remained the same (whether both point to the same address)
	EXPECT_EQ(trafo0Params, (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf9.getRoot().get())->getParameters()));

	//Evaluate
	std::cout << "START EVALUATION FOR 9 TRAFOS\n"; 
	res = sf9.multiEvaluate(directions,true);
	std::cout << "END EVALUATION\n";

	//Check if the supportvalues were right
	//Should be [1 + #trafos , 1] everywhere
	EXPECT_EQ(res.at(0).supportValue, TypeParam(10));
	EXPECT_EQ(res.at(1).supportValue, TypeParam(1));

/*
	int numOfTrafos = 9;
	for(int i = 0; i < numOfTrafos; i++){
		
		sf = sf.affineTransformation(tMat, tVec);
		
		EXPECT_TRUE(sf.getRoot() != nullptr);
		EXPECT_TRUE(sf.getRoot()->getChildren().size() == 1);
		EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf.getRoot().get()))->getCurrentExponent(), 1);
		EXPECT_EQ((dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf.getRoot().get()))->getSuccessiveTransformations(), 0);
		std::cout << "Constructed trafo" << i << std::endl;
		
		if(i == 0){
			//Save pointer to parameters for later, checking if it remains the same object 
			trafo0Params = (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf.getRoot().get()))->getParameters();
			std::cout << "saved trafo0Params with address " << trafo0Params << std::endl;
		}
		
		if((i % 2 == 1) || (i == numOfTrafos)){

			//Check whether parameter object remained the same (whether both point to the same address)
			EXPECT_EQ(trafo0Params, (dynamic_cast<TrafoOp<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>*>(sf.getRoot().get())->getParameters()));

			//Evaluate
			std::cout << "START EVALUATION FOR " << i << " TRAFOS\n"; 
			std::vector<EvaluationResult<TypeParam>> res = sf.multiEvaluate(directions,true);
			std::cout << "END EVALUATION\n";

			//Check if the supportvalues were right
			//Should be [1 + #trafos , 1] everywhere
			EXPECT_EQ(res.at(0).supportValue, TypeParam(1 + (i + 1)));
			EXPECT_EQ(res.at(1).supportValue, TypeParam(1));
		}
	}
*/
}

TYPED_TEST(SupportFunctionNewTest, Constructors){

	//Empty constructor
	std::cout << "Empty constructor test" << std::endl;
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfEmpty;
	EXPECT_TRUE(sfEmpty.empty());
	EXPECT_TRUE(sfEmpty.getRoot() == nullptr);

	//First make a small sf
	std::cout << "Construct leaf" << std::endl;
	Box<TypeParam> box1 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf(&box1);
	EXPECT_EQ(sf.getRoot().use_count(), 1);
	
	std::cout << "Construct sftrafo" << std::endl;
	matrix_t<TypeParam> trafoMat = matrix_t<TypeParam>::Identity(2,2);
	vector_t<TypeParam> trafoVec = vector_t<TypeParam>::Zero(2);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfTrafo = sf.affineTransformation(trafoMat, trafoVec);
	EXPECT_EQ(sfTrafo.getRoot()->getChildren().size(), 1);
	EXPECT_TRUE(&(*(sfTrafo.getRoot()->getChildren().at(0))) == &(*(sf.getRoot())));
	EXPECT_EQ(sfTrafo.getRoot().use_count(), 1);
	EXPECT_EQ(sf.getRoot().use_count(), 2);

	//Copy constructor - copy sfTrafo. the copy should point to the same object sfTrafo points to
	std::cout << "Copy constructor test" << std::endl;
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfCopy(sfTrafo);
	EXPECT_TRUE(&(*(sfCopy.getRoot())) == &(*(sfTrafo.getRoot())));	
	EXPECT_EQ(sfCopy.getRoot()->getChildren().size(), 1);
	EXPECT_EQ(sfTrafo.getRoot()->getChildren().size(), 1);
	EXPECT_TRUE(sfCopy.getRoot()->getChildren().at(0) == sfTrafo.getRoot()->getChildren().at(0));
	EXPECT_EQ(sfTrafo.getRoot().use_count(), 2); 
	EXPECT_EQ(sf.getRoot().use_count(), 2); 

	//Copy assign - set sfCopy to sf. Both sf and sfCopy will then point to the leaf object. 
	std::cout << "Copy Assign test" << std::endl;
	sfCopy = sf;
	EXPECT_TRUE(&(*(sfCopy.getRoot())) == &(*(sf.getRoot())));	
	EXPECT_TRUE(sfCopy.getRoot()->getChildren().size() == 0);
	EXPECT_EQ(sfTrafo.getRoot().use_count(), 1); 
	EXPECT_EQ(sf.getRoot().use_count(), 3); 

	//Move constructor - 
	std::cout << "Move constructor test" << std::endl;
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfMove(std::move(sfCopy));
	EXPECT_TRUE(sfCopy.getRoot() == nullptr); 
	EXPECT_TRUE(&(*(sfMove.getRoot())) == &(*(sf.getRoot())));	
	EXPECT_TRUE(sfMove.getRoot()->getChildren().size() == 0);
	EXPECT_EQ(sfTrafo.getRoot().use_count(), 1); 
	EXPECT_EQ(sf.getRoot().use_count(), 3); 
	//std::cout << "Address of sf root ptr: " << sf.getRoot() << std::endl;
	//std::cout << "Address of sfTrafo root ptr: " << sfTrafo.getRoot() << std::endl;
	//std::cout << "Address of sfCopy root ptr: " << sfCopy.getRoot() << std::endl;
	//std::cout << "Address of sfMove root ptr: " << sfMove.getRoot() << std::endl;

	//Move assign
	std::cout << "Move Assign test" << std::endl;
	sfMove = std::move(sfTrafo);
	EXPECT_TRUE(sfTrafo.getRoot() == nullptr);
	EXPECT_TRUE(sfMove.getRoot() != nullptr);
	EXPECT_TRUE(&(*(sfMove.getRoot()->getChildren().at(0))) == &(*(sf.getRoot())));
	EXPECT_EQ(sfMove.getRoot().use_count(), 1); 
	EXPECT_EQ(sf.getRoot().use_count(), 2); 

}

TYPED_TEST(SupportFunctionNewTest, Deletion){

	//Non-owning pointers needed to check whether the sf's they are pointing to are actually deleted or not
	std::weak_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sf1Copy;
	std::weak_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sf2Copy;
	std::weak_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sfTrafoCopy;
	std::weak_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sfSumCopy;

	{
		//Construct leaf nodes
		Box<TypeParam> box1 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
		Box<TypeParam> box2 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(0)}), Point<TypeParam>({TypeParam(2), TypeParam(2)})));
		std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sf1 = std::make_shared<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&box1);
		std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sf2 = std::make_shared<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&box2);
		sf1Copy = sf1;
		sf2Copy = sf2;
		{
			//Build trafop
			std::cout << "Building trafoOp" << std::endl;
			matrix_t<TypeParam> trafoMat = matrix_t<TypeParam>::Zero(2,2);
			trafoMat(0,0) = TypeParam(2);
			trafoMat(1,1) = TypeParam(2);
			vector_t<TypeParam> trafoVec = vector_t<TypeParam>::Zero(2);

			SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfTrafo = sf1->affineTransformation(trafoMat, trafoVec);
			//std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sfTrafoPtr = std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&sfTrafo);
			std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sfTrafoPtr = std::make_shared<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&sfTrafo);
			sfTrafoCopy = sfTrafoPtr;

			EXPECT_TRUE(sfTrafo.getRoot()->getType() == SFNEW_TYPE::TRAFO);
			EXPECT_TRUE(sfTrafo.getRoot()->getChildren().size() == 1);		

			{
				//Build SumOp
				std::cout << "Building sumOp" << std::endl;
				SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sum = sfTrafo.minkowskiSum(*sf2);
				//std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sumPtr = std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&sum);
				std::shared_ptr<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sumPtr = std::make_shared<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>>(&sum);
				std::cout << "copying shared_ptr" << std::endl;
				sfSumCopy = sumPtr;
				EXPECT_TRUE(sum.getRoot()->getType() == SFNEW_TYPE::SUMOP);
				EXPECT_TRUE(sum.getRoot()->getOriginCount() == sum.getRoot()->getChildren().size());
				std::cout << sum << std::endl;
			}
			//Since SumOp's scope ended, it will be deleted -> sfSumCopy's use_count == 0;
			EXPECT_TRUE(sfSumCopy.expired());
			EXPECT_TRUE(!sfTrafoCopy.expired());
			EXPECT_TRUE(!sf1Copy.expired());
			EXPECT_TRUE(!sf2Copy.expired());
			std::cout << "SumOp expired" << std::endl;
		}	
		EXPECT_TRUE(sfTrafoCopy.expired());
		EXPECT_TRUE(!sf1Copy.expired());
		EXPECT_TRUE(!sf2Copy.expired());	
		std::cout << "TrafoOp expired" << std::endl;
	}
	EXPECT_TRUE(sf1Copy.expired());
	std::cout << "sf1 expired" << std::endl;
	EXPECT_TRUE(sf2Copy.expired());
	std::cout << "sf2 expired" << std::endl;
}

/*
 * Test deletion mechanics. Test cases:
 * 1. Delete supportfunction to leaf (tree stays the same, only sf to leaf vanishes)
 * 2. Delete supportfunction to intermediate node (tree stays the same, only sf to intermediate node vanishes)
 * 3. Delete supportfunction to root (root and its pointer to its children get deleted)
 * 
 * NEEDS COPY ASSIGNMENT
 */
TYPED_TEST(SupportFunctionNewTest, IntermediateDeletion){

	//Construct leaf nodes
	Box<TypeParam> box1 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
	Box<TypeParam> box2 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(0)}), Point<TypeParam>({TypeParam(2), TypeParam(2)})));
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf1(&box1);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf2(&box2);
	
	//Build trafop
	matrix_t<TypeParam> trafoMat = matrix_t<TypeParam>::Identity(2,2);
	vector_t<TypeParam> trafoVec = vector_t<TypeParam>::Zero(2);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfWithTrafo = sf1.affineTransformation(trafoMat, trafoVec);

	EXPECT_TRUE(sfWithTrafo.getRoot()->getType() == SFNEW_TYPE::TRAFO);
	EXPECT_TRUE(sfWithTrafo.getRoot()->getChildren().size() == 1);
	
	//Build SumOp
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sum = sfWithTrafo.minkowskiSum(sf2);
	EXPECT_TRUE(sum.getRoot()->getType() == SFNEW_TYPE::SUMOP);
	EXPECT_TRUE(sum.getRoot()->getOriginCount() == sum.getRoot()->getChildren().size());	
	
	//Vector where all supportFunctions are saved - sf's are saved in deletion order, last one deleted first
	std::cout << "fill sfVec" << std::endl;
	std::vector<SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault>> sfVec;
	sfVec.emplace_back(std::move(sf1));
	sfVec.emplace_back(std::move(sum));
	sfVec.emplace_back(std::move(sfWithTrafo));
	sfVec.emplace_back(std::move(sf2));
	EXPECT_TRUE(sf1.getRoot() == nullptr);
	EXPECT_TRUE(sum.getRoot() == nullptr);
	EXPECT_TRUE(sfWithTrafo.getRoot() == nullptr);
	EXPECT_TRUE(sf2.getRoot() == nullptr);

	//Evaluate
	std::cout << "START EVALUATION\n"; 
	matrix_t<TypeParam> directions = matrix_t<TypeParam>::Identity(2,2);
	std::vector<EvaluationResult<TypeParam>> res = sfVec.at(1).multiEvaluate(directions,true);
	//std::cout << "Result of Evaluation is:\n";
	//for(auto& eRes : res){
	//	std::cout << eRes << std::endl;
	//}
	std::cout << "END EVALUATION\n";

	//Now delete leaf2
	std::cout << "=== Now delete leaf2" << std::endl;
	sfVec.pop_back();
	EXPECT_EQ(sfVec.size(), 3);
	EXPECT_TRUE(sfVec.at(1).getRoot()->getChildren().at(1) != nullptr);
	std::vector<EvaluationResult<TypeParam>> res2 = sfVec.at(1).multiEvaluate(directions,true);
	EXPECT_TRUE(res2.size() == res.size());
	for(std::size_t i=0; i < res2.size(); i++){
		EXPECT_TRUE(res2.at(i) == res.at(i));
	}

	//Now delete a intermediate node
	std::cout << "=== Now delete trafo" << std::endl;
	sfVec.pop_back();
	EXPECT_EQ(sfVec.size(), 2);
	EXPECT_TRUE(sfVec.at(1).getRoot()->getChildren().at(0) != nullptr);
	res2 = sfVec.at(1).multiEvaluate(directions,true);
	EXPECT_TRUE(res2.size() == res.size());
	for(std::size_t i=0; i < res2.size(); i++){
		EXPECT_TRUE(res2.at(i) == res.at(i));
	}	
	
	//Now delete root - this should delete sum, trafo and leaf 2, leaving only leaf1, thus the evaluation result should change
	std::cout << "=== Now delete sum" << std::endl;
	sfVec.pop_back();
	EXPECT_EQ(sfVec.size(), 1);
	res2 = sfVec.at(0).multiEvaluate(directions,true);
	for(std::size_t i=0; i < res2.size(); i++){
		EXPECT_FALSE(res2.at(i) == res.at(i));
	}

}


////// Functionality Tests

TYPED_TEST(SupportFunctionNewTest, Evaluate){
	
	//Construct leaf nodes
	Box<TypeParam> box1 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(-1)}), Point<TypeParam>({TypeParam(1), TypeParam(2)})));
	Box<TypeParam> box2 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(0)}), Point<TypeParam>({TypeParam(2), TypeParam(2)})));

	//Assemble them to a tree 
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf(&box1);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf2(&box2);
	
	//Build trafop
	matrix_t<TypeParam> trafoMat = matrix_t<TypeParam>::Identity(2,2);
	vector_t<TypeParam> trafoVec = vector_t<TypeParam>::Zero(2);

	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sfWithTrafo = sf.affineTransformation(trafoMat, trafoVec);
	
	EXPECT_TRUE(sfWithTrafo.getRoot()->getType() == SFNEW_TYPE::TRAFO);
	EXPECT_TRUE(sfWithTrafo.getRoot()->getChildren().size() == 1);
	
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sum = sfWithTrafo.minkowskiSum(sf2);
	
	EXPECT_TRUE(sum.getRoot()->getType() == SFNEW_TYPE::SUMOP);
	EXPECT_TRUE(sum.getRoot()->getOriginCount() == sum.getRoot()->getChildren().size());
	std::cout << sum << std::endl;

	//Evaluate
	matrix_t<TypeParam> directions = matrix_t<TypeParam>::Zero(2,2);
	directions(0,0) = TypeParam(1);
	directions(1,1) = TypeParam(1);
	//std::cout << "START EVALUATION\n"; 
	std::vector<EvaluationResult<TypeParam>> res = sum.multiEvaluate(directions,true);
	//std::cout << "END EVALUATION\n";
	//std::cout << "Result of Evaluation is:\n";
	//for(auto& eRes : res){
	//	std::cout << eRes << std::endl;
	//}
	
}

TYPED_TEST(SupportFunctionNewTest, Emptyness){


	Box<TypeParam> box1 = Box<TypeParam>::Empty(2);
	Box<TypeParam> box2 (std::make_pair(Point<TypeParam>({TypeParam(0),TypeParam(0)}), Point<TypeParam>({TypeParam(2), TypeParam(2)})));
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf1(&box1);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sf2(&box2);
	EXPECT_TRUE(sf1.empty());
	EXPECT_TRUE(!sf2.empty());

	matrix_t<TypeParam> mat = matrix_t<TypeParam>::Identity(2,2);
	vector_t<TypeParam> vec = vector_t<TypeParam>::Zero(2);
	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> trafo = sf1.affineTransformation(mat, vec);
	EXPECT_TRUE(trafo.empty());	

	SupportFunctionNewT<TypeParam,Converter<TypeParam>,SupportFunctionNewDefault> sum = trafo.minkowskiSum(sf2);
	EXPECT_TRUE(sum.empty());
}