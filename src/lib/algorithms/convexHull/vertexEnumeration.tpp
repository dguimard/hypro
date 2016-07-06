#include "vertexEnumeration.h"
#define CHULL_DBG
namespace hypro {

	template<typename Number>
	void VertexEnumeration<Number>::increment(unsigned& i, unsigned& j, unsigned maxJ) {
		++j; if(j>=maxJ){j=0;++i;};//cout<<"\nincr  i="<<i<<",j="<<j<<"\n";
	}
	
	template<typename Number>
	VertexEnumeration<Number>::VertexEnumeration(const std::vector<Halfspace<Number>>& hsv) {
		mHsv=hsv;
	}
	
	template<typename Number>
	std::vector<Halfspace<Number>> VertexEnumeration<Number>::getHsv() const {return mHsv;}
	
	template<typename Number>
	std::vector<Dictionary<Number>> VertexEnumeration<Number>::getDictionaries() const {return mDictionaries;}
	
	template<typename Number>
	std::vector<Point<Number>> VertexEnumeration<Number>::getPositivePoints() const {return mPositivePoints;}
	template<typename Number>
	void VertexEnumeration<Number>::printPositivePoints() const {
		for(const auto& p:mPositivePoints) {
			cout<<p<<"\n";
		}
		cout<<"\n";
	}
	
	template<typename Number>
	std::vector<Point<Number>> VertexEnumeration<Number>::getPoints() const {return mPoints;}
	template<typename Number>
	void VertexEnumeration<Number>::printPoints() const {
		for(const auto& p:mPoints) {
			cout<<p<<"\n";
		}
		cout<<"\n";
	}
	
	template<typename Number>
	std::vector<vector_t<Number>> VertexEnumeration<Number>::getLinealtySpace() const {return mLinealtySpace;}
	template<typename Number>
	void VertexEnumeration<Number>::printLinealtySpace() const {
	for(const auto& l:mLinealtySpace) {
			cout<<l<<"\n";
		}
		cout<<"\n";
	}
	
	template<typename Number>
	std::vector<vector_t<Number>> VertexEnumeration<Number>::getPositiveCones() const {return mPositiveCones;}
	template<typename Number>
	void VertexEnumeration<Number>::printPositiveCones() const {
	for(const auto& c:mPositiveCones) {
			cout<<c<<"\n";
		}
		cout<<"\n";
	}
	
	template<typename Number>
	std::vector<vector_t<Number>> VertexEnumeration<Number>::getCones() const {return mCones;}
	template<typename Number>
	void VertexEnumeration<Number>::printCones() const {
	for(const auto& c:mCones) {
			cout<<c<<"\n";
		}
		cout<<"\n";
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::enumerateVertices() {
		//findLinealtySpace();
		//addLinealtyConstrains();
		if(findPositiveConstrains()) {
			enumerateDictionaries();
			enumerateVerticesEachDictionary();
			toGeneralCoordinates();
		}
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::enumerateVertices(Dictionary<Number>& dictionary) {
		std::set<vector_t<Number>> cones =dictionary.findCones(); 
		for(const auto& cone: cones) {
			mPositiveCones.insert(cone);
		}
		unsigned a=0;
		unsigned b=0;
		int depth=0;
		unsigned& i=a;
		unsigned& j=b;
		unsigned m = dictionary.basis().size()-1;//different than the article
		unsigned n = dictionary.cobasis().size()-1;
		while(i<m || depth>=0){
			while(i<m && not(dictionary.reverse(i,j))){
				VertexEnumeration<Number>::increment(i,j,n);
			}
			if(i<m){
				dictionary.pivot(i,j);
				if(dictionary.isLexMin()) {
					#ifdef CHULL_DBG
						cout << "\n new point: ";
						//dictionary.printDictionary();
						cout << dictionary.toPoint();
					#endif
					mPositivePoints.push_back(dictionary.toPoint());
				}
				std::set<vector_t<Number>> cones =dictionary.findCones(); 
				for(const auto& cone: cones) {
					#ifdef CHULL_DBG
						cout << "cone found ---------------------------------------";
					#endif
					mPositiveCones.insert(cone);
				}
				i=0;j=0;++depth;
			} else { 
				if(depth>0) {
					dictionary.selectBlandPivot(i,j);
					dictionary.pivot(i,j);
				}
				VertexEnumeration<Number>::increment(i,j,n);
				--depth;
			} 
		}
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::enumerateVerticesEachDictionary() {
		mPositivePoints.push_back(mDictionaries[0].toPoint());
		for(unsigned i = 0; i<mDictionaries.size(); ++i) {
			#ifdef CHULL_DBG
				cout<< "\n\n Next dictionary ---------------------\n";
				mDictionaries[i].printDictionary();
			#endif
			enumerateVertices(mDictionaries[i]);
		}
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::enumerateDictionaries() {
		Dictionary<Number> dictionary = mDictionaries[0];
		unsigned a=0;
		unsigned b=0;
		int depth=0;
		std::vector<unsigned> basisAux = dictionary.findZeros();//locate degenerated variables
		std::vector<Number> memory;
		unsigned& i=a;
		unsigned& j=b;
		unsigned m = basisAux.size();//different than the article
		unsigned m2 = dictionary.basis().size()-1;
		unsigned n = dictionary.cobasis().size()-1;
		for(unsigned rowIndex = 0; rowIndex <= m2; ++rowIndex) {
			memory.push_back(dictionary.get(rowIndex,n));
		}
		dictionary.setOnes(basisAux);
		while(i<m || depth>=0){
			while(i<m && not(dictionary.reverseDual(i,j,basisAux))){
				VertexEnumeration<Number>::increment(i,j,n);
			}
			if(i<m){
				dictionary.pivot(basisAux[i],j);
				Dictionary<Number> newDictionary = (Dictionary<Number>(dictionary));
				for(unsigned rowIndex = 0; rowIndex <= m2; ++rowIndex) {
					newDictionary.setValue(rowIndex,n,memory[rowIndex]);
				}
				mDictionaries.push_back(newDictionary);
				i=0;j=0;++depth;
			} else {
				if(depth>0) {
					dictionary.selectDualBlandPivot(i,j,basisAux);
					dictionary.pivot(basisAux[i],j);
				}
				VertexEnumeration<Number>::increment(i,j,n);
				--depth;
			} 
		}
		#ifdef CHULL_DBG
			cout<<"\n nb dico = " << mDictionaries.size() <<"\n";
		#endif
	}
	
	template<typename Number>
	Dictionary<Number> VertexEnumeration<Number>::findFirstVertex() {
		unsigned d = mHsv[0].dimension();
		unsigned n0 = mHsv.size();
		Dictionary<Number> dictionary = Dictionary<Number>(mHsv);
		while(dictionary.fixOutOfBounds()) {}
		dictionary.nonSlackToBase(mLinealtySpace);
		addLinealtyConstrains();
		matrix_t<Number> dictio = matrix_t<Number>::Zero(mHsv.size()+1, d+1);
		for(unsigned colIndex=0;colIndex<=d;++colIndex) {//copy
			for(unsigned rowIndex=0;rowIndex<n0;++rowIndex) {
				dictio(rowIndex,colIndex) = dictionary.get(rowIndex,colIndex);
			}
			dictio(mHsv.size(),colIndex) = dictionary.get(n0,colIndex);
		}
		for(unsigned rowIndex=0;rowIndex<n0;++rowIndex) {//build the linealty constrains, the part with the var in the basis
			if(dictionary.basis()[rowIndex]>=dictionary.basis().size()) {
				for(unsigned colIndex=0;colIndex<d;++colIndex) {
					for(unsigned rowIndexLinealty=0;rowIndexLinealty<mLinealtySpace.size();++rowIndexLinealty) {
						dictio(n0+2*rowIndexLinealty,colIndex)-=
								dictionary.get(rowIndex,colIndex)*mLinealtySpace[rowIndexLinealty][dictionary.basis()[rowIndex]-dictionary.basis().size()];
					}
				}
			}
		}
		for(unsigned colIndex=0;colIndex<d;++colIndex) {//build the linealty constrains, the part with the var in the cobasis
			for(unsigned rowIndexLinealty=0;rowIndexLinealty<mLinealtySpace.size();++rowIndexLinealty) {
				if(dictionary.cobasis()[colIndex]>=dictionary.basis().size()) {
					dictio(n0+2*rowIndexLinealty,colIndex)-=mLinealtySpace[rowIndexLinealty][colIndex];
				}
				dictio(n0+2*rowIndexLinealty+1,colIndex) = -dictio(n0+2*rowIndexLinealty,colIndex);
			}
		}
		std::vector<std::size_t> basis = dictionary.basis();
		std::vector<std::size_t> cobasis = dictionary.cobasis();
		for(auto& index: basis) {
			if(index>n0) {index+=2*mLinealtySpace.size();};
		}
		for(auto& index: cobasis) {
			if(index>n0) {index+=2*mLinealtySpace.size();};
		}
		std::size_t back = basis.back();
		basis.pop_back();
		
		for(unsigned index = n0+1;index<=mHsv.size();++index) {basis.push_back(index);}
		basis.push_back(back);
		ConstrainSet<Number> constrains;
		for(unsigned index=0; index<n0;++index) {
			constrains.add(dictionary.constrainSet().get(index));
		}
		for(unsigned index=0; index<2*mLinealtySpace.size();++index) {
			constrains.add(std::tuple<std::pair<bool,Number>,std::pair<bool,Number>,Number>(
					std::pair<bool,Number>(false,Number(0)),std::pair<bool,Number>(true,Number(0)),Number(0)));//fix
		}
		for(unsigned index=n0; index<n0+d;++index) {
			constrains.add(dictionary.constrainSet().get(index));
		}
		/*std::size_t zero = 0;
		constrains.modifyAssignment (zero,zero,basis, cobasis, dictio);
		for(unsigned rowIndexLinealty=0;rowIndexLinealty<mLinealtySpace.size();++rowIndexLinealty) {
			constrains.setLowerBoundToValue(n0+2*rowIndexLinealty);
			constrains.setLowerBoundToValue(n0+2*rowIndexLinealty+1);
		}*/
		Dictionary<Number> newDictionary = Dictionary<Number>(dictio,basis,cobasis,constrains);
		while(newDictionary.fixOutOfBounds()) {}
		#ifdef CHULL_DBG
			cout <<"\nthe new dico\n";
			newDictionary.printDictionary();
			newDictionary.constrainSet().print();
			cout <<"\n\n\n";
		#endif
		newDictionary.nonSlackToBase();
		std::set<unsigned> hyperplanes;
		for(unsigned index=0;index<basis.size();++index) {
			if(newDictionary.constrainSet().isSaturated(index)) {
				hyperplanes.insert(index);
			}
		}
		std::set<unsigned> frozenCols = newDictionary.toCobase(hyperplanes);
		for(unsigned colIndex=0; colIndex<d;++colIndex) {
			if(frozenCols.end()==frozenCols.find(colIndex)) {
				newDictionary.pushToBounds(colIndex);
			}
		}
		return newDictionary;
	}
	
	template<typename Number>
	int VertexEnumeration<Number>::linearIndependance(std::map<int,vector_t<Number>> collection, vector_t<Number>& candidateRef) const {
		for(typename std::map<int,vector_t<Number>>::iterator it=collection.begin(); it!=collection.end(); ++it) {
			Number ratio = (candidateRef[it->first])/(it->second)[it->first];
			for(int i=0;i<candidateRef.size();++i) {
				candidateRef[i] -= ratio*it->second[i];
			}
		}
		int nonNulIndex = -1;
		int i=0;
		while(nonNulIndex<0 && i<candidateRef.size()) {
			if(candidateRef[i]!=0) {
				return i;
			}
			++i;
		}
		return nonNulIndex;
	}
	
	template<typename Number>
	std::vector<unsigned> VertexEnumeration<Number>::findIndepHs() const {
		unsigned dim = mHsv[0].dimension();
		unsigned count = 0;
		unsigned index = 0;
		std::vector<unsigned> selection;
		std::map<int,vector_t<Number>> collection;
		while(count<dim) {//assuming there are enought independant halfspaces
			vector_t<Number> candidate = mHsv[index].normal();
			vector_t<Number>& candidateRef = candidate;
			int nonNulIndex = linearIndependance(collection, candidateRef);
			if(nonNulIndex!=-1) {
				collection.insert(std::pair<int,vector_t<Number>>(nonNulIndex,candidate));
				++count;
				selection.push_back(index);
			}
			++index;
		}
		return selection;
	}
	
	template<typename Number>
	Point<Number> VertexEnumeration<Number>::findIntersection(const std::vector<unsigned>& selectionRef) const {
		unsigned dimension = selectionRef.size();
		matrix_t<Number> mat = matrix_t<Number>(dimension, dimension);
		vector_t<Number> vect = vector_t<Number>(dimension);
		for(unsigned rowIndex=0; rowIndex<dimension; ++rowIndex) {
			for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
				mat(rowIndex,colIndex) = (mHsv[selectionRef[rowIndex]]).normal()[colIndex];
			}
		vect[rowIndex] = (mHsv[selectionRef[rowIndex]]).offset();
		}
		return Point<Number>(mat.fullPivLu().solve(vect));
	}
	
	
	template<typename Number>
	bool VertexEnumeration<Number>::findPositiveConstrains() {
		try{
			#ifdef CHULL_DBG
				Dictionary<Number>dictionary(findFirstVertex());
				dictionary.printDictionary();
				dictionary.constrainSet().print();
				cout<<"end\n";
			#endif
			unsigned dimension = dictionary.cobasis().size()-1;
			unsigned constrainsCount = dictionary.basis().size()-1;
			matrix_t<Number> A1 = matrix_t<Number>(dimension, dimension);
			matrix_t<Number> A2 = matrix_t<Number>(mHsv.size()-dimension, dimension);
			vector_t<Number> b1 = vector_t<Number>(dimension);
			std::vector<std::size_t> mB;
			std::vector<std::size_t> mN;
			for(unsigned rowIndex=0; rowIndex<dimension; ++rowIndex) {
				for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
					A1(rowIndex,colIndex) = (mHsv[dictionary.cobasis()[rowIndex]-1]).normal()[colIndex];
				}
				b1[rowIndex] = (mHsv[dictionary.cobasis()[rowIndex]-1]).offset();
			}
			matrix_t<Number> invA1 = A1.inverse();
			unsigned skiped = 0;
			for(unsigned rowIndex=0; rowIndex<constrainsCount; ++rowIndex) {
				if(dictionary.basis()[rowIndex]-1<constrainsCount) {
					for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
						A2(rowIndex-skiped,colIndex) = (mHsv[dictionary.basis()[rowIndex]-1]).normal()[colIndex];
					} 
				} else {++skiped;}
			}
			matrix_t<Number> newDictionary = matrix_t<Number>(constrainsCount-dimension+1, dimension+1);//faire la derniere ligne
			skiped = 0;
			for(unsigned rowIndex=0; rowIndex<constrainsCount; ++rowIndex) {
				if(dictionary.basis()[rowIndex]-1<constrainsCount) {
					for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
					newDictionary(rowIndex-skiped,colIndex)=0;
						for(unsigned index=0; index<dimension; ++index) {
							newDictionary(rowIndex-skiped,colIndex)+=A2(rowIndex-skiped,index)*invA1(index,colIndex);
						}
					}
					newDictionary(rowIndex-skiped,dimension)= (mHsv[dictionary.basis()[rowIndex]-1]).offset();
					for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
						newDictionary(rowIndex-skiped,dimension)-=newDictionary(rowIndex-skiped,colIndex)*b1[colIndex];
					}
				} else {++skiped;}
			}
		
			for(unsigned colIndex=0; colIndex<dimension; ++colIndex) {
				newDictionary(constrainsCount-dimension,colIndex)=Number(-1);
			}
		
			for(unsigned i=1; i<constrainsCount-dimension+1; ++i){
				mB.push_back(std::size_t(i));
			}
			mB.push_back(std::size_t(constrainsCount+1));
			for(unsigned i=constrainsCount-dimension+1; i<constrainsCount+1; ++i){
				mN.push_back(std::size_t(i));
			}
			mN.push_back(std::size_t(constrainsCount+2));
		
			mDictionaries.push_back(Dictionary<Number>(newDictionary,mB,mN));
		
			mPivotingMatrix = invA1;
			mOffset = b1;
		} 
		catch(string const& message) {
			cout << message;
			return false;
		}
		return true;
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::toGeneralCoordinates() {
		for(unsigned index=0; index<mPositivePoints.size(); ++index) {
			mPoints.push_back(Point<Number>(mPivotingMatrix*(mOffset-mPositivePoints[index].rawCoordinates())));
		}
		for(const auto& cone: mPositiveCones) {
			mCones.push_back((mPivotingMatrix*(-cone)));
		}
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::findLinealtySpace() {
		unsigned dim = mHsv[0].dimension();
		unsigned count = 0;
		unsigned index = 0;
		std::vector<Number> norms;
		std::vector<vector_t<Number>> collection;
		while(count<dim&&index<mHsv.size()) {
			vector_t<Number> candidate = mHsv[index].normal();
			for(unsigned vectorIndex=0;vectorIndex<count;++vectorIndex){
				candidate=candidate-collection[vectorIndex]*((collection[vectorIndex].dot(candidate)/norms[vectorIndex]));
			}
			int i=0;
			while(i<candidate.size()) {
				if(candidate[i]!=0) {break;}
				++i;
			}
			if(i<candidate.size()) {
				collection.push_back(candidate);
				norms.push_back(candidate.dot(candidate));
				++count;
			}
			++index;
		}
		index = 0;
		unsigned linealtySpaceSize = dim-count;		
		while(count<dim) {
			vector_t<Number> baseVector = vector_t<Number>::Zero(dim);
			baseVector[index]=Number(-1);
			for(unsigned vectorIndex=0;vectorIndex<count;++vectorIndex){
				baseVector=baseVector-collection[vectorIndex]*(collection[vectorIndex].dot(baseVector)/norms[vectorIndex]);
			}
			int i=0;
			while(i<baseVector.size()) {
				if(baseVector[i]!=0) {break;}
				++i;
			}
			if(i<baseVector.size()) {
				collection.push_back(baseVector);
				norms.push_back(baseVector.dot(baseVector));
				++count;
			}
			++index;
		}
		for(index = 0; index<linealtySpaceSize; ++index) {
			mLinealtySpace.push_back(collection[dim-index-1]);
		}
	}
	
	template<typename Number>
	void VertexEnumeration<Number>::addLinealtyConstrains() {
		for(unsigned linealtyIndex=0;linealtyIndex<mLinealtySpace.size();++linealtyIndex) {
			mHsv.push_back(Halfspace<Number>(mLinealtySpace[linealtyIndex],Number(0)));
			mHsv.push_back(Halfspace<Number>(Number(-1)*mLinealtySpace[linealtyIndex],Number(0)));
		}
	}
	
} // namespace