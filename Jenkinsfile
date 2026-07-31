// Jenkinsfile – RenderLoopLab declarative pipeline
// Demonstrates a CI pipeline suitable for an internal Jenkins server.

pipeline {
    agent any

    options {
        timeout(time: 30, unit: 'MINUTES')
        buildDiscarder(logRotator(numToKeepStr: '10'))
    }

    stages {

        stage('Checkout') {
            steps {
                checkout scm
            }
        }

        stage('Configure') {
            steps {
                script {
                    if (isUnix()) {
                        sh '''
                            cmake -B build -G Ninja \
                              -DCMAKE_BUILD_TYPE=Release \
                              -DRLL_BUILD_TESTS=ON \
                              -DRLL_WARNINGS_AS_ERRORS=ON
                        '''
                    } else {
                        bat 'cmake -B build -DCMAKE_BUILD_TYPE=Release -DRLL_BUILD_TESTS=ON -DRLL_WARNINGS_AS_ERRORS=ON'
                    }
                }
            }
        }

        stage('Build') {
            steps {
                script {
                    if (isUnix()) {
                        sh 'cmake --build build --parallel'
                    } else {
                        bat 'cmake --build build --config Release --parallel'
                    }
                }
            }
        }

        stage('Unit Test') {
            steps {
                script {
                    if (isUnix()) {
                        sh 'ctest --test-dir build --output-on-failure'
                    } else {
                        bat 'ctest --test-dir build -C Release --output-on-failure'
                    }
                }
            }
        }

        stage('Runtime Smoke Test') {
            when {
                // Only run runtime stages on Unix agents where xvfb is available
                expression { return isUnix() }
            }
            steps {
                sh '''
                    if command -v xvfb-run >/dev/null 2>&1; then
                        xvfb-run -a ./build/bin/renderlooplab \
                          --smoke-test --frames 60 --hidden --sprites 100
                    else
                        echo "xvfb-run not available; skipping runtime smoke test."
                    fi
                '''
            }
        }

        stage('Short Benchmark') {
            when {
                expression { return isUnix() }
            }
            steps {
                sh '''
                    if command -v xvfb-run >/dev/null 2>&1; then
                        xvfb-run -a ./build/bin/renderlooplab \
                          --benchmark \
                          --sprites 200 \
                          --frames 120 \
                          --warmup 30 \
                          --no-vsync \
                          --hidden \
                          --csv benchmark.csv \
                          --report benchmark.json
                    else
                        echo "xvfb-run not available; skipping benchmark."
                    fi
                '''
            }
        }

        stage('Archive Artifacts') {
            steps {
                archiveArtifacts artifacts: 'benchmark.csv, benchmark.json',
                                 allowEmptyArchive: true
            }
        }
    }

    post {
        failure {
            echo 'Build or test failed — check the console output above.'
        }
        success {
            echo 'Pipeline completed successfully.'
        }
    }
}
