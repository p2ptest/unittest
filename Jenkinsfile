pipeline {
  agent {
    docker {
      image 'python:3-alpine'
    }
  }
  
  parameters {
    choice(name: ‘PerformMavenRelease’, choices: ‘False\nTrue’, description: 'desc')
    password(name: 'CredsToUse', description: 'A password to build with', defaultValue: '')
  }
  environment {
    BUILD_USR_CHOICE = "${params.PerformMavenRelease}"
    BUILD_USR_CREDS = "${params.CredsToUse}"
  }  
  stages {
    stage('test') {
      steps {
        sh 'python --version'
        echo $BUILD_USR_CHOICE $BUILD_USR_CREDS
      }
    }
  }
}
