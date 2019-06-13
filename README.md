# von
Visualization of Neural network /vɒn/

* 신경망 시각화([참고](http://playground.tensorflow.org/))와 유사하지만 더 interactive하게 데이터를 추가하고 신경망 모델을 바꿀 수 있음
* winapi, C++ 사용
* 개요  
  <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_img.png" width="700"/></p>  
  * 신경망에서 input layer의 다음 layer의 뉴런 하나는 input space상에서 하나의 hyperplane에 대응  
  * activation function에 따라 차이는 있지만 입력값(이전 레이어의 출력값과 가중치의 선형 결합)이 0인 곳을 결정경계로 삼을 수 있음  
  * input layer에서 좀 더 멀리 떨어진 layer에 속하는 각 뉴런은 input space에 어떤 결정경계를 만들지, 이 뉴런들의 출력값을 각 입력에 대해 plotting 하면 어떤 개형을 보일지 확인하기 위한 프로그램  
* 실행 화면  
  * 다양한 패턴의 2차원 데이터를 직접 입력으로 넣어볼 수 있고 실시간으로 학습함  
    input 뉴런을 빨간색으로, output 뉴런을 초록색으로 표시  
    input 뉴런 두 개에 (x1, x2) 값이 주어졌을때 output 뉴런 값이 1에 가까울 수록 흰색, 0에 가까울 수록 검은색으로 Plot 창에 그려짐  
    <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_0to4.png" width="700"/></p>
    <br />  
  * hidden layer의 특정 뉴런을 output 뉴런으로 삼아서 가능한 input 각각의 경우들에 대해 어떤 출력값을 가지는지 확인 가능  
    <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_0to2.png" width="700"/></p>
    <br />  
  * 개요에 언급한 input layer의 다음 layer의 뉴런 하나가 어떤 hyper plane에 대응하는지 (이 경우 2차원상의 hyperplane이므로 직선) 확인 가능(해당 뉴런을 하늘색으로 표시)  
    아래 이미지에 원래는 직선 4개가 보여야하지만 직선 하나가 plot 되고 있는 영역 밖에 있어 3개만 표시  
    (어느 뉴런이 불필요한 뉴런인지 직관적으로 알 수 있음. 지금과 같은 경우 삼각형 모양의 결정경계를 만들기 위해선 input layer의 다음 layer가 뉴런 3개로만 구성되어도 충분하다는 사실을 알 수 있음)  
    <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_0to4with1.png" width="700"/></p>
    <br />  
  * hidden layer의 결정 경계도 확인 가능  
    <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_0to4with2_1.png" width="700"/></p>
    <br />  
  * 신경망 구조가 꼭 층 형태가 아니더라도 backprop이 가능하게 짬  
    <p align="center"><img src="https://raw.githubusercontent.com/mori-inj/temp_md/master/von_nonlayer.png" width="700"/></p>
    layer를 무시하고 DAG 형태로 신경망을 만들때의 결정 경계 확인 가능  
    <br />  
* 은닉층의 결정경계가 어떻게 되는지, 은닉층을 독립된 입력으로 볼때 출력층과의 관계가 어떻게 되는지 등을 확인 가능
* 추가 예정  
  * 3차원상에서의 결정 경계(현재는 input 뉴런으로 삼을 수 있는 뉴런이 2개뿐)  
  * 현재는 input space 상에서 input으로 삼은 뉴런의 이후 뉴런들이 어떤 결정경계와 함숫값을 가지느냐의 관점에서 접근하고 있다. (즉, input space는 선형(Identical)이고, 결정경계가 비선형)  
    신경망을 layer 단위로 본다면 어떤 데이터를 선형 변환 후 activation function을 씌워 다른 공간의 데이터로 보내는 것으로 생각할 수도 있다. ([이 경우 data가 비선형 변환을 거쳐 다른 공간에 매핑되고, 결정경계는 공간 상에서 두 데이터 집단을 선형으로 분리](http://colah.github.io/posts/2014-03-NN-Manifolds-Topology/))  
    각 layer를 거치며 data가 분포하는 공간이 어떻게 찌그러지고 늘려지는지 시각화([예시 데모](http://cs.stanford.edu/people/karpathy/convnetjs/demo/classify2d.html ))  
