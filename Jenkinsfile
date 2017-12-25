pipeline {
  agent {
    docker {
      image 'python:3-alpine'
    }
    
  }
  stages {
    stage('test') {
      steps {
        sh 'python --version'
      }
    }
  }
}