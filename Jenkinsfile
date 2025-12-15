pipeline {
    agent any
    
    environment {
        CPPTEST_HOME = 'C:\\CPP_STD_25-2\\cpptest'
        PATH = "C:\\mingw64\\mingw64\\bin;${env.PATH}"
    }
    
    stages {
        stage('Checkout') {
            steps {
                echo "Pulling latest code from GitHub"
                git branch: 'main', url: 'https://github.com/zuwasi/timer-demo'
            }
        }
        
        stage('Build') {
            steps {
                echo "Building timer application with CMake"
                bat """
                    if exist build rmdir /s /q build
                    mkdir build
                    cd build
                    cmake ..
                    cmake --build . --config Release
                """
            }
        }
        
        stage('Parasoft Analysis') {
            steps {
                echo "Running Parasoft C++test analysis"
                bat """
                    "%CPPTEST_HOME%\\cpptestcli.exe" ^
                    -localsettings "%WORKSPACE%\\cpptestcli.properties" ^
                    -config "builtin://Recommended Rules" ^
                    -compiler gcc_11-64 ^
                    -module . ^
                    -include "%WORKSPACE%\\*.c" ^
                    -include "%WORKSPACE%\\*.h" ^
                    -report "%WORKSPACE%\\reports"
                """
            }
        }
    }
    
    post {
        always {
            archiveArtifacts artifacts: 'reports/**/*', allowEmptyArchive: true
        }
        success {
            echo 'Build and analysis completed successfully!'
        }
        failure {
            echo 'Build or analysis failed!'
        }
    }
}
