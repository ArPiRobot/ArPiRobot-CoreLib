#include <arpirobot/core/drive.hpp>
#include <arpirobot/core/log.hpp>

#include <cmath>

using namespace arpirobot;

////////////////////////////////////////////////////////////////////////////////
/// ArcadeDriveHelper
////////////////////////////////////////////////////////////////////////////////

ArcadeDriveHelper::ArcadeDriveHelper(MotorController *leftMotor, MotorController *rightMotor){
    this->leftMotors.push_back(leftMotor);
    this->rightMotors.push_back(rightMotor);
}

ArcadeDriveHelper::ArcadeDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors){
    for(auto &motor : leftMotors){
        this->leftMotors.push_back(motor);
    }
    for(auto &motor : rightMotors){
        this->rightMotors.push_back(motor);
    }
}

void ArcadeDriveHelper::updateSpeed(double newSpeed){
    speed = newSpeed;
    doUpdate();
}

void ArcadeDriveHelper::updateRotation(double newRotation){
    rotation = newRotation;
    doUpdate();
}

void ArcadeDriveHelper::update(double newSpeed, double newRotation){
    speed = newSpeed;
    rotation = newRotation;
    doUpdate();
}

std::array<double, 2> ArcadeDriveHelper::calculateSpeeds(double speed, double rotation){
    double left = 0, right = 0;

    // Don't allow speed or rotation to be negative zero
    speed += 0.0;
    rotation += 0.0;

    double maxInput = std::copysign(std::max(std::abs(speed), std::abs(rotation)), speed);

    if((rotation >= 0 && speed >= 0) || (rotation < 0 && speed < 0)){
        left = maxInput;
        right = speed - rotation;
    }else{
        left = speed + rotation;
        right = maxInput;
    }

    return {left, right};
}

void ArcadeDriveHelper::doUpdate(){
    // Restrict range of speed and rotation
    if(speed > 1)
        speed = 1;
    if(speed < -1)
        speed = -1;
    if(rotation > 1)
        rotation = 1;
    if(rotation < -1)
        rotation = -1;
    
    auto speeds = calculateSpeeds(speed, rotation);

    for(auto motor : leftMotors){
        motor->setSpeed(speeds[0]);
    }
    for(auto motor : rightMotors){
        motor->setSpeed(speeds[1]);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// TankDriveHelper
////////////////////////////////////////////////////////////////////////////////

TankDriveHelper::TankDriveHelper(MotorController *leftMotor, MotorController *rightMotor){
    this->leftMotors.push_back(leftMotor);
    this->rightMotors.push_back(rightMotor);
}

TankDriveHelper::TankDriveHelper(std::vector<MotorController*> leftMotors, std::vector<MotorController*> rightMotors){
    for(auto &motor : leftMotors){
        this->leftMotors.push_back(motor);
    }
    for(auto &motor : rightMotors){
        this->rightMotors.push_back(motor);
    }
}

void TankDriveHelper::updateLeftSpeed(double newLeftSpeed){
    leftSpeed = newLeftSpeed;
    doUpdate();
}

void TankDriveHelper::updateRightSpeed(double newRightSpeed){
    rightSpeed = newRightSpeed;
    doUpdate();
}

void TankDriveHelper::update(double newLeftSpeed, double newRightSpeed){
    leftSpeed = newLeftSpeed;
    rightSpeed = newRightSpeed;
    doUpdate();
}

void TankDriveHelper::doUpdate(){
    // Restrict speed ranges
    if(leftSpeed > 1)
        leftSpeed = 1;
    if(leftSpeed < -1)
        leftSpeed = -1;
    if(rightSpeed > 1)
        rightSpeed = 1;
    if(rightSpeed < -1)
        rightSpeed = -1;

    for(auto motor : leftMotors){
        motor->setSpeed(leftSpeed);
    }
    for(auto motor : rightMotors){
        motor->setSpeed(rightSpeed);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// SquareRootAxisTransform
////////////////////////////////////////////////////////////////////////////////

double SquareRootAxisTransform::applyTransform(double rawAxisValue){
    double y = std::abs(rawAxisValue);
    if(rawAxisValue < 0)
        return -1 * y;
    return y;
}


////////////////////////////////////////////////////////////////////////////////
/// CubicAxisTransform
////////////////////////////////////////////////////////////////////////////////

CubicAxisTransform::CubicAxisTransform(double minPower, double midPower){
    minPower = std::abs(minPower);
    midPower = std::abs(midPower);

    std::vector<double> x = {0, 0.5, 1};
    std::vector<double> y = {minPower, midPower, 1};
    auto coeffs = polyfit(x, y, 3);
    
    a = coeffs[3];
    b = coeffs[2];
    c = coeffs[1];
    d = coeffs[0];
}

double CubicAxisTransform::applyTransform(double rawAxisValue){
    double x = std::abs(rawAxisValue);
    double y = a * std::pow(x, 3) + b * std::pow(x, 2) + c * x + d;
    if(rawAxisValue < 0)
        return y * -1;
    return y;
}

// Based on MIT Licensed code from https://gist.github.com/chrisengelsma/108f7ab0a746323beaaf7d6634cf4add
std::vector<double> CubicAxisTransform::polyfit(std::vector<double> x, std::vector<double> y, int order){
	if (x.size() != y.size()) {
		throw std::runtime_error( "Polyfit cannot work with different x and y array sizes!" );
	}
	if (x.size() == 0 || y.size() == 0) {
		throw std::runtime_error( "Polyfit cannot work with x or y arrays with a size of 0!" );
	}

	size_t N = x.size();
	int n = order;
	int np1 = n + 1;
	int np2 = n + 2;
	int tnp1 = 2 * n + 1;
	double tmp;

	// X = vector that stores values of sigma(xi^2n)
	std::vector<double> X(tnp1);
	for (int i = 0; i < tnp1; ++i) {
		X[i] = 0;
		for (size_t j = 0; j < N; ++j)
			X[i] += (double)pow(x[j], i);
	}

	// a = vector to store final coefficients.
	std::vector<double> a(np1);

	// B = normal augmented matrix that stores the equations.
	std::vector<std::vector<double> > B(np1, std::vector<double> (np2, 0));

	for (int i = 0; i <= n; ++i)
		for (int j = 0; j <= n; ++j)
			B[i][j] = X[i + j];

	// Y = vector to store values of sigma(xi^n * yi)
	std::vector<double> Y(np1);
	for (int i = 0; i < np1; ++i) {
		Y[i] = (double)0;
		for (size_t j = 0; j < N; ++j) {
			Y[i] += (double)pow(x[j], i)*y[j];
		}
	}

	// Load values of Y as last column of B
	for (int i = 0; i <= n; ++i)
		B[i][np1] = Y[i];

	n += 1;
	int nm1 = n-1;

	// Pivotisation of the B matrix.
	for (int i = 0; i < n; ++i)
		for (int k = i+1; k < n; ++k)
			if (B[i][i] < B[k][i])
				for (int j = 0; j <= n; ++j) {
					tmp = B[i][j];
					B[i][j] = B[k][j];
					B[k][j] = tmp;
				}

	// Performs the Gaussian elimination.
	// (1) Make all elements below the pivot equals to zero
	//     or eliminate the variable.
	for (int i=0; i<nm1; ++i)
		for (int k =i+1; k<n; ++k) {
			double t = B[k][i] / B[i][i];
			for (int j=0; j<=n; ++j)
				B[k][j] -= t*B[i][j];         // (1)
	}

	// Back substitution.
	// (1) Set the variable as the rhs of last equation
	// (2) Subtract all lhs values except the target coefficient.
	// (3) Divide rhs by coefficient of variable being calculated.
	for (int i=nm1; i >= 0; --i) {
		a[i] = B[i][n];                   // (1)
		for (int j = 0; j<n; ++j)
			if (j != i)
				a[i] -= B[i][j] * a[j];       // (2)
		a[i] /= B[i][i];                  // (3)
	}

    std::vector<double> coeffs;
	coeffs.resize(a.size());
	for (size_t i = 0; i < a.size(); ++i)
		coeffs.insert(coeffs.begin() + i, a[i]);

	return coeffs;
}